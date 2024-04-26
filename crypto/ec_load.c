#include "hblk_crypto.h"

/**
* ec_load - load an EC key pair from the disk
* @folder: path to folder from which to load the keys
*
* Return: pointer to created EC key pair, NULL on error
*/
EC_KEY *ec_load(char const *folder)
{
	EC_KEY *key = NULL;
	char buf[BUFSIZ];
	FILE *filePointer;

	/*Check if folder is NULL*/
	if (!folder || strlen(folder) + strlen(PUB_FILENAME) > BUFSIZ)
		return (NULL);
	/*Create a new EC_KEY structure*/
	sprintf(buf, "%s/%s", folder, PUB_FILENAME);
	filePointer = fopen(buf, "r");
	/*Check if filePointer is NULL*/
	if (!filePointer)
		return (NULL);
	/*Read the public key from the file*/
	if (!PEM_read_EC_PUBKEY(filePointer, &key, NULL, NULL))
	{
		fclose(filePointer);
		return (NULL);
	}

	fclose(filePointer);
	/*Create a new EC_KEY structure*/
	sprintf(buf, "%s/%s", folder, PRI_FILENAME);
	filePointer = fopen(buf, "r");
	if (!filePointer)
	{
		EC_KEY_free(key);
		return (NULL);
	}
	/*Read the private key from the file*/
	if (!PEM_read_ECPrivateKey(filePointer, &key, NULL, NULL))
	{
		fclose(filePointer);
		EC_KEY_free(key);
		return (NULL);
	}

	fclose(filePointer);
	return (key);
}
