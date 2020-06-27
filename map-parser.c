#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_NODES 64
#define MAX_NUMBER_VERTICES 256

#define CURRENT_VERSION "v4"

enum Errors
{
    NO_ERRORS,
    INCORRECT_VERSION,
    NOT_IMPLEMENTED_YET,
    UNKNOWN_MAP_TYPE
};

enum MapType
{
    NV,
    MATRIX,
};

enum MapFlags
{
    F = 1,
    N = 2,
    S = 4,
    W = 8,
    A = 16,
    X = 32,
};

struct node
{
    char *name;
};

struct vertice
{
    char *name;  // Should be there but unused
    struct node from;
    struct node to;
    int weight;
};

struct mapNVData
{
    int flags;
    int nodeSize;
    int verticeSize;

    struct node nodes[MAX_NUMBER_NODES];
    struct vertice vertices[MAX_NUMBER_VERTICES];

    struct node first_last[2];
    char *algo;
    char *description;
};

struct Map
{
    int error;
    int is_matrix;
    struct mapNVData mapNV;
    //  struct mapMatrixData mapData;
};

int getFlagNum(char flag[2]);
void printNVMap(struct mapNVData map);
struct Map readFile(char *fname);

int main(void)
{   
    struct Map m = readFile("map_nv.map");
   // struct Map m = parseMap("map_nv.map");
    printNVMap(m.mapNV);
    return 0;
}

int getFlagNum(char flag[2]) // very redundant but cant really do better
{
    if (flag[strlen(flag) - 1] == '\n')
    {
        flag[strlen(flag) - 1] = '\0';
    }
    if (strcmp(flag, "F") == 0)
    {
        return F;
    }
    if (strcmp(flag, "N") == 0)
    {
        return N;
    }
    if (strcmp(flag, "S") == 0)
    {
        return S;
    }
    if (strcmp(flag, "W") == 0)
    {
        return W;
    }
    if (strcmp(flag, "A") == 0)
    {
        return A;
    }
    if (strcmp(flag, "X") == 0)
    {
        return X;
    }
    printf("\nWARNING: Parser encountered an unknown flag: %s\n", flag);
    return 0;
}

void printNVMap(struct mapNVData map)
{
    printf("\n======================================\n");
    printf("MAP METADATA:\n");
    printf("\tFlags: %d\n", map.flags);
    printf("\tNumber of nodes: %d\n", map.nodeSize);
    printf("\tNumber of vertices: %d\n", map.verticeSize);
    printf("\tMap recommended algo: %s\n", map.algo);
    printf("\tMap descrition: %s\n", map.description);

    printf("\nMAP NODES:\n");
    for (int i = 0; i < map.nodeSize; i++)
    {
        printf("\tNode #%d: %s\n", i, map.nodes[i].name);
    }
    printf("\tFirst/Last nodes: %s %s\n", map.first_last[0].name, map.first_last[1].name);
    printf("\nMAP VERTICES:\n");
    for (int i = 0; i < map.verticeSize; i++)
    {
        printf("\tVertice #%d:\n", i);
        printf("\t\tFrom node: %s\n", map.vertices[i].from.name);
        printf("\t\tTo node:   %s\n", map.vertices[i].to.name);
        printf("\t\tWeight:    %d\n", map.vertices[i].weight);
    }

    printf("======================================\n");
}

struct Map readFile(char *fname)
{
    /* OPEN FILE */
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;
    FILE *fp = fopen(fname, "r");
    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", fname);
    }

    /* GET VERSION NUMBER */
    char *version;
    line_size = getline(&version, &line_buf_size, fp);
    printf("%s\n", version);
    version[strlen(version) - 1] = 0;
    if (strcmp(version, CURRENT_VERSION) != 0)
    {
        printf("VERSION NOT SUPPORTED BY THIS PROGRAM\n");
        struct Map map;
        map.error = INCORRECT_VERSION;
        return map;
    }

    /* GET MAP TYPE */
    char *type;
    line_size = getline(&type, &line_buf_size, fp);
    type[strlen(type) - 1] = 0; // GET rid = \n
    if (strcmp(type, "NV") == 0)
    {
        printf("Parsing NV map\n");

        struct mapNVData map;

        /* GET FLAGS */
        printf("Getting flags\n");
        char *flags;
        line_size = getline(&flags, &line_buf_size, fp);
        char *token = strtok(flags, " ");
        int flags_value = 0;
        while (token != NULL)
        {
            int val = getFlagNum(token);
            flags_value = (flags_value | val); // BITWISE OR
            token = strtok(NULL, " ");
        }
        map.flags = flags_value;

        /* GET NODES/VERTICES SIZE */
        printf("Getting nodes/vertices size\n");
        int node_size;
        char *temp;
        line_size = getline(&temp, &line_buf_size, fp);
        node_size = atoi(temp);
        map.nodeSize = node_size;

        int vertice_size;
        line_size = getline(&temp, &line_buf_size, fp);
        vertice_size = atoi(temp);
        map.verticeSize = vertice_size;
        
        /* GET NODES */
        for (int i = 0; i < map.nodeSize; i++)
        {
            char *name;
            line_size = getline(&map.nodes[i].name, &line_buf_size, fp);
            map.nodes[i].name[strlen(map.nodes[i].name) - 1] = '\0';
        }

        /* GET VERTICES */
        struct vertice vertices[map.verticeSize];
        for (int i = 0; i < map.verticeSize; i++)
        {
            struct vertice adding;
            char *data;
            line_size = getline(&data, &line_buf_size, fp);
            data[strlen(data) - 1] = '\0';

            char * token;
            
            map.vertices[i].name = strtok(data, " ");
            printf("Stuff: %s\n", map.vertices[i].name);
            adding.name = token;
            
            token = strtok(NULL, " ");
            char *node_name = token;
            for (int j = 0; j < map.nodeSize; j++)
            {
                if (strcmp(map.nodes[j].name, node_name) == 0)
                {
                    adding.from = map.nodes[j];
                    break;
                }
            }
            token = strtok(NULL, " ");

            char *to_node_name = token;
            for (int j = 0; j < map.nodeSize; j++)
            {
                if (strcmp(map.nodes[j].name, to_node_name) == 0)
                {
                    adding.to = map.nodes[j];
                    break;
                }
            }
            token = strtok(NULL, " ");
           // token[strlen(token) - 1] = '\0';
            adding.weight = atoi(token);
            map.vertices[i].from = adding.from;
            map.vertices[i].to = adding.to;
            map.vertices[i].weight = adding.weight;
        }
        
        /* GET START/FINISH */
        printf("Getting start / finish\n");
        char *start_finish;
        getline(&start_finish, &line_buf_size, fp);
        token = strtok(start_finish, " ");
        for (int j = 0; j < map.nodeSize; j++)
        {
            if (strcmp(map.nodes[j].name, token) == 0)
            {
                map.first_last[0] = map.nodes[j];
                break;
            }
        }
        token = strtok(NULL, " ");
        token[strlen(token) - 1] = '\0';
        for (int j = 0; j < map.nodeSize; j++)
        {
            if (strcmp(map.nodes[j].name, token) == 0)
            {
                map.first_last[1] = map.nodes[j];
                break;
            }
        }
        getline(&map.algo, &line_buf_size, fp);
        map.algo[strlen(map.algo) - 1] = '\0';

        getline(&map.description, &line_buf_size, fp);
        map.description[strlen(map.description) - 1] = '\0';

        fclose(fp);
        struct Map return_map;
        return_map.is_matrix = 1; // false
        return_map.mapNV = map;
        return return_map;
    }
    else if (strcmp(type, "MATRIX") == 0)
    {
        printf("NOT IMPLEMENTED YET\n");
        struct Map map;
        map.error = NOT_IMPLEMENTED_YET;
        return map;
    }
    else
    {
        printf("UNKNOWN TYPE\n");
        struct Map map;
        map.error = UNKNOWN_MAP_TYPE;
        return map;
    }

    

    /* Free the allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);
}
