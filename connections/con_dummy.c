#include "../rcp_pch.h"
#include "../rcp_defines.h"
#include "../rcp_buffer.h"


#include "../rcp_io.h"
#include "../rcp_receiver.h"
#include "../rcp_sender.h"

void cmp_dummy_init(void* userdata);
void cmp_dummy_deinit(void* userdata);
void cmp_dummy_build_l0(void* userdata, 
		rcp_type_ref type, rcp_data_ref data);
void cmp_dummy_build_l1(void* userdata, 
		const uint8_t *begin, const uint8_t *end);
void cmp_dummy_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end);
void cmp_dummy_twist(void* userdata);
void cmp_dummy_clean_up(void* userdata);

struct rcp_sender_l0_class cmp_dummy_l0_class = {
	0,
	cmp_dummy_init,
	cmp_dummy_deinit,
	cmp_dummy_build_l0,
	cmp_dummy_result,
	cmp_dummy_clean_up
};

struct rcp_sender_l1_class cmp_dummy_l1_class = {
	0,
	cmp_dummy_init,
	cmp_dummy_deinit,
	cmp_dummy_build_l1,
	cmp_dummy_result,
	cmp_dummy_twist,
	cmp_dummy_clean_up
};

void cmp_dummy_init(void* userdata)
{ }
void cmp_dummy_deinit(void* userdata)
{ }
void cmp_dummy_build_l0(void* userdata, 
		rcp_type_ref type, rcp_data_ref data)
{ }
void cmp_dummy_build_l1(void* userdata, 
		const uint8_t *begin, const uint8_t *end)
{ }
void cmp_dummy_result(void* userdata, 
		const uint8_t **begin, const uint8_t **end)
{
	*begin = NULL;
	*end = NULL;
}
void cmp_dummy_twist(void* userdata)
{ }
void cmp_dummy_clean_up(void* userdata)
{ }
