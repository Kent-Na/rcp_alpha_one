#include "rcp_pch.h"
#include "rcp_utility.h"
#include "rcp_defines.h"

#include "rcp_ssl.h"

///
//ssl
//

static SSL_CTX *ctx=NULL;

void rcp_ssl_ctx_init()
{
	const SSL_METHOD *method = SSLv3_server_method();
	if (method == NULL){
		rcp_error("e1\n");
	}
	ctx = SSL_CTX_new(method);
	if (ctx== NULL){
		rcp_error("e2\n");
	}
	SSL_CTX_use_certificate_file(ctx, 
			RCP_SSL_CERTIFICATE_FILE_PATH, SSL_FILETYPE_PEM);
	SSL_CTX_use_PrivateKey_file(ctx, 
			RCP_SSL_PRIVATEKEY_FILE_PATH, SSL_FILETYPE_PEM);
}

SSL_CTX* rcp_ssl_ctx()
{
	return ctx;
}
