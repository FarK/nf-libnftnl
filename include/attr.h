#ifndef _LIBNFTNL_ATTR_INTERNAL_H_
#define _LIBNFTNL_ATTR_INTERNAL_H_

#include <stdint.h>
#include <stddef.h>

/*
 * TLV structures:
 * nftnl_attr
 *  <-------- HEADER --------> <------ PAYLOAD ------>
 * +------------+-------------+- - - - - - - - - - - -+
 * |    type    |     len     |         value         |
 * |  (1 byte)  |   (1 byte)  |                       |
 * +--------------------------+- - - - - - - - - - - -+
 *  <-- sizeof(nftnl_attr) --> <-- nftnl_attr->len -->
 *  <------------- nftnl_attr_get_size() ------------>
 */
struct __attribute__((__packed__)) nftnl_attr {
	uint8_t type;
	uint8_t len;
	unsigned char value[];
};

/*
 *              +-------------------------------------------++
 *              |           data[]                          ||
 *              |             ||                            ||
 *              |             \/                            \/
 *  +-------+-------+- - - - -+-------+-------+ ... +-------+- - - - - - -+
 *  | size  |  end  | padding |  TLV  |  TLV  |     |  TLV  |    Empty    |
 *  +-------+-------+- - - - -+-------+-------+ ... +-------+- - - - - - -+
 *                            |<- nftnl_attrbuf_get_len() ->|
 *                            |<-------- nftnl_attrbuf_get_size() ------->|
 */
struct nftnl_attrbuf {
	size_t size;
	char  *end;
	char   data[] __attribute__((aligned(64)));
};

#endif
