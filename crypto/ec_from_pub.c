#include "hblk_crypto.h"

/**
* ec_from_pub - Crea una estructura EC_KEY dada una clave pública
*
* @pub: Clave pública para ser convertida
*
* Return: Puntero a la estructura EC_KEY creada en caso de éxito,
* NULL en caso de fallo
*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key;
	EC_POINT *point;
	const EC_GROUP *group;

	if (!pub)
		return (NULL);
	/*Create a new EC_KEY structure*/
	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (!key)
	{
		fprintf(stderr, "Failed to create EC key\n"), EC_KEY_free(key);
		return (NULL);
	}
	/*Get the group from the EC_KEY structure*/
	group = EC_KEY_get0_group(key);
	if (!group)
	{
		fprintf(stderr, "Failed to get EC group\n"), EC_KEY_free(key);
		return (NULL);
	}
	point = EC_POINT_new(group);
	if (!point)
	{
		fprintf(stderr, "Failed to create EC point\n");
		EC_KEY_free(key);
		return (NULL);
	}
	/*Assign the public key to the EC_POINT point*/
	if (EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL) != 1)
		return fprintf(stderr, "Failed to convert octet to EC point\n"),
						EC_KEY_free(key), EC_POINT_free(point), NULL;
	/* Asignar el punto EC_POINT a la estructura EC_KEY */
	if (EC_KEY_set_public_key(key, point) != 1)
	{
		fprintf(stderr, "Failed to set public key in EC key\n"), EC_KEY_free(key);
		EC_POINT_free(point);
		return (NULL);
	}
	EC_POINT_free(point);
	return (key);
}
