#define PRODUCT_DELAY		250	/* ms */
#define MAX_DESTINATION		3

#define PID_FILE		"/tmp/dispatcher.pid"

typedef struct payload_t {
	unsigned char destination;
	int data;
	
} payload_t;

typedef enum mq_list_t {
	DISPATCHER,
	BUFFER,
	DESTINATION,
	
} mq_list_t;

char *mq_names[] = {
	"/dispatcher",
	"/dispatch_buffer",
	"/destination",
};

size_t mq_sizes[] = {
	10,
	10000,
	10,
};
