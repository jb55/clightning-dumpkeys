
#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H


int descriptor_checksum(const char *descriptor, int desc_size, char *checksum,
			int checksum_capacity);

#endif /* DESCRIPTOR_H */
