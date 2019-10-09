#include "SUSE.h"

void _suse_init () {
	t_config *config = config_create ("Config.config");
	int *vals = malloc (4 * sizeof (config_get_array_value (config, "Vals")));
	int *maxs = malloc (4 * sizeof (config_get_array_value (config, "Maxs")));
	char *ids = malloc (sizeof (config_get_array_value (config, "Ids")));

	printf ("%i\n", sizeof (config_get_array_value (config, "Vals")));

	*vals = config_get_array_value (config, "Vals");

	*maxs = config_get_array_value (config, "Maxs");

	*ids = config_get_array_value (config, "Ids");

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

	for (int i = 0; i < 3; i++) {
		printf ("%c, %i, %i\n", repo -> id, repo -> val, repo -> max);

		repo = repo -> sig;
	}
}

void main () {
	_suse_init ();
}
