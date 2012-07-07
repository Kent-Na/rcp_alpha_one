#include "def/rcp_data.h"
#include "def/rcp_type.h"
#include "def/rcp_sender_l0.h"
#include "def/rcp_sender_l1.h"

struct rcp_sender_terget{
	rcp_data_ref data;
	rcp_type_ref type;
};

///
//layer 0 (data to byte array)

struct rcp_sender_l0_class{
	size_t size;

	void (*init)(void *userdata);

	void (*deinit)(void *userdata);

	void (*build)(void *userdata, rcp_type_ref type, rcp_data_ref data);

	void (*result)(void *userdata, 
			const uint8_t **begin, const uint8_t **end);

	void (*clean_up)(void *userdata);
};

void* rcp_sender_l0_userdata(rcp_sender_l0_ref);

rcp_sender_l0_ref rcp_sender_l0_new(
		struct rcp_sender_l0_class *klass,
		struct rcp_sender_terget *tgt);

void rcp_sender_l0_clean_up(rcp_sender_l0_ref l0);

///
//layer 1 (byte array to frame)
struct rcp_sender_l1_class{
	size_t size;

	void (*init)(void *userdata);

	void (*deinit)(void *userdata);

	void (*build)(void *userdata, 
			const uint8_t *begin, const uint8_t *end);

	void (*result)(void *userdata, 
			const uint8_t **begin, const uint8_t **end);

	//twist web socket bit mask for each connection
	void (*twist)(void *userdata);

	void (*clean_up)(void *userdata);
};

rcp_sender_l1_ref rcp_sender_l1_new(
		struct rcp_sender_l1_class *klass,
		rcp_sender_l0_ref l0);

void rcp_sender_l1_clean_up(rcp_sender_l1_ref l1);

void rcp_sender_result(rcp_sender_l1_ref cmp, 
		const uint8_t **begin, const uint8_t **end);


