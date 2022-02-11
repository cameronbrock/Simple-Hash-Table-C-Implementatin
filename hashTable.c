
#define VALUE_TYPE (void*)

// Struct containing the key as a string constant, and
// the value as a VALUE_TYPE. It also contains a pointer
// to another key-value pair just in case there is a collision
// and we have to insert two key-value pairs in the same index!
typedef struct {
	const char* key; // String value of the key
	VALUE_TYPE value; // pointer to the element being stored.
	char* nextKvPair; // Next key-value pair in the list if applicable.
} KeyValuePair;

// This is the hash table itself. It is made up of an array
// of keyValuePair* pointers, each of which is either NULL (meaning
// there is no key-value pair stored there) or contains a pointer to
// the first key-value pair that was inserted  at that index.
typedef struct {
	KeyValuePair** keyValues;
	unsigned int numTableElements;
} HashTable;

// Given a key-value, returns an index in the array.
unsigned int hash(HashTable* table, const char* key) {
	
	unsigned int hashVal = 0;
	for (unsigned int i=0; key[i] != '\0'; i++) {
		char c = key[i];
		hashVal += c * c;
	}

	hashVal %= table->numTableElements;
	return hashVal;
	
}

// Returns the value given the key-value.
VALUE_TYPE hashTableLookup(HashTable* hashTable, const char* key) {
	
	unsigned int keyValuesIndex = hash(hashTable, key);
	KeyValuePair* kVal = hashTable->keyValues[keyValuesIndex];
	
	// If the first key-value pair in the current index has the same
	// key as the one we're looking for, return the value. Otherwise,
	// traverse the linked list looking for whichever key-value pair
	// matches our key.
	if (strcmp(kVal->key, key) == 0) {
		return kVal->value;
	}
	while (kVal != NULL) {
		kVal = (KeyValuePair*) kVal->nextKvPair;
		if (strcmp(kVal->key, key) == 0) {
			return kVal->value;
		}
	}
	
}

// Insert a value into the hash table.
void hashTableInsert(HashTable* hashTable, const char* key, VALUE_TYPE value) {

	unsigned int keyValuesIndex = hash(hashTable, key);
	printf("Index = %d / %d\n", keyValuesIndex, hashTable->numTableElements);
	
	// Now create a new KeyValuePair object.
	KeyValuePair* kvPair = (KeyValuePair*) malloc(sizeof(KeyValuePair));
	kvPair->key = key;
	kvPair->value = value;
	kvPair->nextKvPair = NULL;
	
	if (hashTable->keyValues[keyValuesIndex] == NULL) {
		hashTable->keyValues[keyValuesIndex] = kvPair;
		return;
	}
	
	KeyValuePair* currentPair = hashTable->keyValues[keyValuesIndex];
	while (currentPair->nextKvPair != NULL) {
		currentPair = (KeyValuePair*) currentPair->nextKvPair;
	}
	
	currentPair->nextKvPair = (char*) kvPair;
	
	return;
	
}

// Generate the hash table data structure.
HashTable* hashTableInit(unsigned int numTableElements) {
	
	// Increase numTableElements by the power of 2. This
	// reduces the possibility of collisions.
	numTableElements *= numTableElements;
	
	HashTable* table = (HashTable*) malloc(sizeof(HashTable));
	table->keyValues = (KeyValuePair**) malloc(numTableElements * sizeof(KeyValuePair*));
	table->numTableElements = numTableElements;
	
	for (int i=0; i<numTableElements; i++) {
		table->keyValues[i] = NULL;
	}
	
	return table;
	
}

