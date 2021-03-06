//connection
#define RCP_RAW_JSON_PORT_NUMBER (4000)
#define RCP_WS_JSON_PORT_NUMBER (4001)
#define RCP_LISTEN_SOCKET_NUMBER (10)
#define RCP_PROTOCOL_JSON_BUFFER_SIZE (4096)
#define RCP_SEND_TIMEOUT_SEC (2)
#define RCP_OPEN_TIMEOUT_SEC (10)

#define RCP_INIT_FILE ("init_rcp")
#define RCP_INTERNAL_CONNECTION_NAME ("tuna-cat")

//ssl
#define RCP_SSL_CERTIFICATE_FILE_PATH ("key/certificate.pem")
#define RCP_SSL_PRIVATEKEY_FILE_PATH ("key/key.pem")

//password
#define RCP_SERVER_SALT ("tIIMJNHnjkfKsPiH")

//database
#define RCP_DB_USER ("czel")
#define RCP_DB_PASSWORD ("")
#define RCP_DB_NAME ("rcp")

//if defined this macro, run time self test will be execute.
#define RCP_SELF_TEST (1)
