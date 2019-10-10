#include "SUSE.h"

void _suse_init () {
	t_config *config = config_create ("Cosas/Config.config");
	//int vals [] = malloc (4 * sizeof (config_get_array_value (config, "Vals")) - 4);
	//int maxs [] = malloc (4 * sizeof (config_get_array_value (config, "Maxs")) - 4);
	//char ids [] = malloc (sizeof (config_get_array_value (config, "Ids")) - 1);

	int *vals = config_get_array_value (config, "Vals");

	int *maxs = config_get_array_value (config, "Maxs");

	char *ids = config_get_array_value (config, "Ids");

	for (int i = 0; i < sizeof (config_get_array_value (config, "Vals")); i++) {
		struct semaforo *sem = malloc (sizeof (struct semaforo));

		sem -> sig = repo;

		sem -> id = (*ids + i);
		printf ("%c, %c\n", sem -> id, ids [i]);

		sem -> max = (*maxs + i * 4);
		printf ("%i, %i\n", sem -> max, maxs [i]);

		sem -> val = (*vals + i * 4);
		printf ("%i, %i\n", sem -> val, vals [i]);

		repo = sem;
	}
}

void main () {
	_suse_init ();
}
