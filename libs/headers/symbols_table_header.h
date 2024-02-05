typedef struct node * symbolNodePtr;
typedef struct node{
	char name[MAX_LINE_SIZE]; /* MAX LINE SIZE = 80 */
	int value;
	int types[AMOUNT_OF_TYPES]; /* AMOUNT OF TYPES = 4 */
	symbolNodePtr next;
}symbol;

static symbolNodePtr symbolTableHead = NULL;
