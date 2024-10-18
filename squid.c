#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <curl/curl.h>


// Client //

typedef struct {
    char *bit_range;
    long private_key;
    char *private_key_hex;
} Payload;

typedef struct {
    char *url;
    char *token;
} SimpleClient;

void init_client(SimpleClient *client) {
    client->url = getenv("API_ENDPOINT");
    client->token = getenv("API_AUTH_TOKEN");

    if (client->url == NULL) {
        client->url = "";
        printf("\n[!] Empty URL!\n");
    }

    if (client->token == NULL) {
        client->token = "";
        printf("\n[!] Empty token!\n");
    }
}

CURLcode simple_client_post(SimpleClient *client, Payload *payload) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;

        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        
        char auth_header[256];
        snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", client->token);
        headers = curl_slist_append(headers, auth_header);

        curl_easy_setopt(curl, CURLOPT_URL, client->url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        char json_payload[512];
        snprintf(
            json_payload, 
            sizeof(json_payload), 
            "{\"_bit_range\": \"%s\", \"_private_key\": %ld, \"_private_key_hex\": \"%s\"}", 
            payload->bit_range, payload->private_key, payload->private_key_hex
        );
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    
    return res;
}

void handle_response(CURLcode res) {
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("\n[+] Key successfully sent to the server!\n");
    }
}

// File checker //

int fileExistenceChecker(const char *fileName) {
    struct stat buffer;
    return (stat(fileName, &buffer) == 0);
}

void fileContent(const char *filename, char *bitRange) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("Error trying to read the file content!");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "Key found privkey") != NULL) {
            char *key = strrchr(buffer, ' ');
            if (key != NULL) {
                key++;
                
                uint64_t private_key = strtoull(key, NULL, 16);
                char keyHex[65];
                snprintf(keyHex, 65, "%064llx", (unsigned long long)private_key);
                
                printf("[+] Found Key: %s", key);
                printf("[+] Key HEX: %s\n", keyHex);

                SimpleClient client;
                Payload payload;
                
                init_client(&client);

                payload.bit_range = bitRange;
                payload.private_key = private_key;
                payload.private_key_hex = keyHex;
                
                CURLcode res = simple_client_post(&client, &payload);

                handle_response(res);
            }
            break;
        }
    }

    fclose(file);
}

int main() {
    const char *fileName = "KEYFOUNDKEYFOUND.txt";
    
    int targetRange;
    char bitRange[50];
    
    const short counter = 1;
    long checkCount = 0;

    printf("_bit_range: ");
    scanf("%d", &targetRange);
    
    snprintf(bitRange, sizeof(bitRange), "2^%d...2^%d-1", targetRange - 1, targetRange);

    printf("\n[+] Waiting for the bit range key '%s'\n\n", bitRange);

    while (counter) {
        if (fileExistenceChecker(fileName)) {
            printf("\n\n[+] Found file: '%s'\n\n", fileName);
            
            fileContent(fileName, bitRange);
            
            return 0;
        } else {
            printf("\r[-] Nothing here yet! | check_count: %ld", checkCount + 1);
            fflush(stdout);
            
            sleep(5);
            checkCount++;
        }
    }    
}
