#ifndef _LIBNFTNL_ATTR_H_
#define _LIBNFTNL_ATTR_H_

#include <stdio.h>
#include <stdint.h>

/*
 * nftnl attributes API
 */
struct nftnl_attr;
struct nftnl_attrbuf;

/* nftnl_attrbuf */
struct nftnl_attrbuf *nftnl_attrbuf_alloc(size_t data_size);
void nftnl_attrbuf_free(struct nftnl_attrbuf *attrbuf);
size_t nftnl_attrbuf_get_len(const struct nftnl_attrbuf *attrbuf);
size_t nftnl_attrbuf_get_size(const struct nftnl_attrbuf *attrbuf);
void *nftnl_attrbuf_get_data(const struct nftnl_attrbuf *attrbuf);
void nftnl_attrbuf_copy_data(struct nftnl_attrbuf *attrbuf,
			     const void *data, size_t len);
struct nftnl_attr *nftnl_attrbuf_get_start(const struct nftnl_attrbuf *attrbuf);
struct nftnl_attr *nftnl_attrbuf_get_end(const struct nftnl_attrbuf *attrbuf);

/* TLV attribute getters */
uint8_t nftnl_attr_get_type(const struct nftnl_attr *attr);
uint8_t nftnl_attr_get_len(const struct nftnl_attr *attr);
void *nftnl_attr_get_value(const struct nftnl_attr *attr);
uint16_t nftnl_attr_get_size(const struct nftnl_attr *attr);

/* TLV attribute putters */
struct nftnl_attr *nftnl_attr_put(struct nftnl_attrbuf *attrbuf,
				  uint8_t type, uint8_t len, const void *value);
struct nftnl_attr *nftnl_attr_put_check(struct nftnl_attrbuf *attrbuf,
					uint8_t type, size_t len,
					const void *value);

enum nftnl_attr_data_type {
	NFTNL_ATTR_TYPE_COMMENT,
	__NFTNL_ATTR_TYPE_MAX,
};
#define NFTNL_ATTR_TYPE_MAX (__NFTNL_ATTR_TYPE_MAX - 1)

/* TLV iterators */
struct nftnl_attr *nftnl_attr_next(const struct nftnl_attr *attr);

#define nftnl_attr_for_each(attr, attrbuf)                              \
	for ((attr) = nftnl_attrbuf_get_start(attrbuf);                 \
	     (char *)(nftnl_attrbuf_get_end(attrbuf)) > (char *)(attr); \
	     (attr) = nftnl_attr_next(attr))

/* TLV callback-based attribute parsers */
#define NFTNL_CB_ERROR	-1
#define NFTNL_CB_STOP	 0
#define NFTNL_CB_OK	 1

typedef int (*nftnl_attr_cb_t)(const struct nftnl_attr *attr, void *data);
int nftnl_attr_parse(const struct nftnl_attrbuf *attrbuf, nftnl_attr_cb_t cb,
		     void *data);

#endif /* _LIBNFTNL_ATTR_H_ */
