#include <libnftnl/attr.h>
#include <attr.h>
#include <utils.h>

#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>


struct nftnl_attrbuf *nftnl_attrbuf_alloc(size_t data_size)
{
	struct nftnl_attrbuf *attrbuf;

	attrbuf = (struct nftnl_attrbuf *)
		malloc(sizeof(struct nftnl_attrbuf) + data_size);
	attrbuf->size = data_size;
	attrbuf->end = attrbuf->data;

	return attrbuf;
}
EXPORT_SYMBOL(nftnl_attrbuf_alloc);

void nftnl_attrbuf_free(struct nftnl_attrbuf *attrbuf)
{
	attrbuf->size = 0;
	attrbuf->end = NULL;
	free((void *)attrbuf);
}
EXPORT_SYMBOL(nftnl_attrbuf_free);

size_t nftnl_attrbuf_get_len(const struct nftnl_attrbuf *attrbuf)
{
	return (size_t)(attrbuf->end - attrbuf->data);
}
EXPORT_SYMBOL(nftnl_attrbuf_get_len);

size_t nftnl_attrbuf_get_size(const struct nftnl_attrbuf *attrbuf)
{
	return attrbuf->size;
}
EXPORT_SYMBOL(nftnl_attrbuf_get_size);

struct nftnl_attr *nftnl_attrbuf_get_start(const struct nftnl_attrbuf *attrbuf)
{
	return (struct nftnl_attr *)attrbuf->data;
}
EXPORT_SYMBOL(nftnl_attrbuf_get_start);

struct nftnl_attr *nftnl_attrbuf_get_end(const struct nftnl_attrbuf *attrbuf)
{
	return (struct nftnl_attr *)attrbuf->end;
}
EXPORT_SYMBOL(nftnl_attrbuf_get_end);

void *nftnl_attrbuf_get_data(const struct nftnl_attrbuf *attrbuf)
{
	return (void *)attrbuf->data;
}
EXPORT_SYMBOL(nftnl_attrbuf_get_data);

void nftnl_attrbuf_copy_data(struct nftnl_attrbuf *attrbuf,
			    const void *data, size_t len)
{
	memcpy(attrbuf->data, data, len <= attrbuf->size? len : attrbuf->size);
	attrbuf->end = attrbuf->data + len;
}
EXPORT_SYMBOL(nftnl_attrbuf_copy_data);

uint8_t nftnl_attr_get_type(const struct nftnl_attr *attr)
{
	return attr->type;
}
EXPORT_SYMBOL(nftnl_attr_get_type);

uint8_t nftnl_attr_get_len(const struct nftnl_attr *attr)
{
	return attr->len;
}
EXPORT_SYMBOL(nftnl_attr_get_len);

void *nftnl_attr_get_value(const struct nftnl_attr *attr)
{
	return (void *)attr->value;
}
EXPORT_SYMBOL(nftnl_attr_get_value);

uint16_t nftnl_attr_get_size(const struct nftnl_attr *attr)
{
	return (char *)nftnl_attr_next(attr) - (char *)attr;
}
EXPORT_SYMBOL(nftnl_attr_get_size);

struct nftnl_attr *nftnl_attr_put(struct nftnl_attrbuf *attrbuf,
				  uint8_t type, uint8_t len, const void *value)
{
	struct nftnl_attr *attr = (struct nftnl_attr *)attrbuf->end;

	attr->len  = len;
	attr->type = type;
	memcpy(attr->value, value, len);

	attrbuf->end = (char *)nftnl_attr_next(attr);

	return attr;
}
EXPORT_SYMBOL(nftnl_attr_put);

struct nftnl_attr *nftnl_attr_put_check(struct nftnl_attrbuf *attrbuf,
					uint8_t type, size_t len,
					const void *value)
{
	// Check if there is enough space
	if (attrbuf->size < len + sizeof(struct nftnl_attr))
		return NULL;

	return nftnl_attr_put(attrbuf, type, len, value);
}
EXPORT_SYMBOL(nftnl_attr_put_check);

struct nftnl_attr *nftnl_attr_next(const struct nftnl_attr *attr)
{
	return (struct nftnl_attr *)&attr->value[attr->len];
}
EXPORT_SYMBOL(nftnl_attr_next);

int nftnl_attr_parse(const struct nftnl_attrbuf *attrbuf, nftnl_attr_cb_t cb,
		     void *data)
{
	int ret = NFTNL_CB_OK;
	const struct nftnl_attr *attr;

	nftnl_attr_for_each(attr, attrbuf)
		if ((ret = cb(attr, data)) <= NFTNL_CB_STOP)
			return ret;
	return ret;
}
EXPORT_SYMBOL(nftnl_attr_parse);
