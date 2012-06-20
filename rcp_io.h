typedef struct rcp_io_core *rcp_io_ref;

struct rcp_io_core{
	struct rcp_io_class *klass;
};

struct rcp_io_class{
	size_t size;
	void (*init)(rcp_io_ref io);
	void (*deinit)(rcp_io_ref io);
	size_t (*send)(rcp_io_ref io, const void *data, size_t len);
	size_t (*receive)(rcp_io_ref io, void *data, size_t len);
	int (*alive)(rcp_io_ref io);
	void (*on_close)(rcp_io_ref io);
};

static __inline__ rcp_io_ref rcp_io_new(struct rcp_io_class *klass)
{
	rcp_io_ref io = malloc(sizeof (struct rcp_io_core) + klass->size);
	io->klass = klass;
	io->klass->init(io);
	return io;
}

static __inline__ void rcp_io_delete(rcp_io_ref io)
{
	io->klass->deinit(io);
	free(io);
}

static __inline__ void* rcp_io_data(rcp_io_ref io)
{
	return io + 1;
}
static __inline__ size_t rcp_io_send(rcp_io_ref io, const void *data, size_t len)
{
	return io->klass->send(io, data, len);
}

static __inline__ size_t rcp_io_receive(rcp_io_ref io, void *data, size_t len)
{
	return io->klass->receive(io, data, len);
}

static __inline__ int rcp_io_alive(rcp_io_ref io)
{
	return io->klass->alive(io);
}

static __inline__ void rcp_io_on_close(rcp_io_ref io)
{
	io->klass->on_close(io);
}
