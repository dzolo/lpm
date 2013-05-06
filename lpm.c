/*
 * FIT VUT - PDS PROJECT 2012/2013 - LONGEST PREFIX MATCH
 * (c) Ondrej Fibich <xfibic01@stud.fit.vutbr.cz>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "trie.h"

/**
 * Parses arguments of the program if arguments are not valid program ends.
 * 
 * @param argv program arguments
 * @param argc 
 * @return readable file that contains network addresses
 */
FILE *parseArgs(int argc, char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Invalid arguments count. Usage: %s -i <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-i") != 0)
    {
        fprintf(stderr, "Wrong arguments. Usage: %s -i <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(argv[2], "r");

    if (f == NULL)
    {
        fprintf(stderr, "Invalid file: %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    return f;
}

/**
 * Main trigger method that is called with path to file that contains addresses
 * of networks.
 */
int main(int argc, char** argv)
{
    // get arguments
    FILE *na = parseArgs(argc, argv);
    // variables
    size_t len = 0, len2;
    trie_t t6 = {}, t4 = {};
    const char *dspace = " ", *dsub = "/";
    char *line = NULL, *subnet = NULL, *as = NULL, *ip = NULL, *cidr = NULL;
    uint8_t ncidr, ipv4[4], ipv6[16], ipv4_on;
    int nas;
    char stdout_buffer[65536];

    // for faster printf
    setvbuf(stdout, stdout_buffer, _IOFBF, sizeof(stdout_buffer));
    
    // initialize tries
    trie_init(&t4);
    trie_init(&t6);

    // load networks
    while (getline(&line, &len, na) != -1)
    {
        // parse line
        subnet = strtok(line, dspace);
        as = strtok(NULL, dspace);
        
        if (subnet == NULL || as == NULL)
        {
            fprintf(stderr, "Invalid network: %s\n", line);
            continue;
        }
        
        ip = strtok(subnet, dsub);
        cidr = strtok(NULL, dsub);
        
        if (ip == NULL || cidr == NULL)
        {
            fprintf(stderr, "Invalid network: %s\n", line);
            continue;
        }
        
        // parse as and cidr
        nas = atoi(as);
        ncidr = atoi(cidr);
        
        if (nas <= 0)
        {
            fprintf(stderr, "Invalid network: %s\n", line);
            continue;
        }
        
        // parse IP
        ipv4_on = 1;
        
        if (inet_pton(AF_INET, ip, &ipv4) != 1) // ipv4
        {
            ipv4_on = 0;
            
            if (inet_pton(AF_INET6, ip, &ipv6) != 1) // ipv6
            {
                fprintf(stderr, "Cannot parse IP: %s\n", ip);
                continue;
            }
        }
        
        // add to trie
        if (ipv4_on)
        {
            trie_node_put(&t4, ipv4, ncidr, nas);
        }
        else
        {
            trie_node_put(&t6, ipv6, ncidr, nas);
        }
    }
    
    // clean
    fclose(na);
    
    // input
    while ((len2 = getline(&line, &len, stdin)) != -1)
    {
        // throw \n away
        if (len2  > 0 && line[len2 - 1] == '\n')
        {
            line[len2 - 1] = 0;
        }
        
        // parse IP
        ipv4_on = 1;
        
        if (inet_pton(AF_INET, ip, &ipv4) != 1) // ipv4
        {
            ipv4_on = 0;
            
            if (inet_pton(AF_INET6, ip, &ipv6) != 1) // ipv6
            {
                fprintf(stderr, "Cannot parse IP: %s\n", ip);
                continue;
            }
        }
        
        // search in trie
        if (ipv4_on)
        {
            if (trie_node_search(&t4, ipv4, 8*4, &nas))
            {
                printf("%d\n", nas);
            }
            else
            {
                printf("-\n");
            }
        }
        else
        {
            if (trie_node_search(&t6, ipv6, 8*16, &nas))
            {
                printf("%d\n", nas);
            }
            else
            {
                printf("-\n");
            }
        }
    }
    
    trie_destroy(&t4);
    trie_destroy(&t6);
    free(line);

    return (EXIT_SUCCESS);
}

