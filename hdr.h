// IP头部
struct iphdr{
	unsigned int ihl:4; //首部长
	unsigned int version:4;  //版本号4位

	u_int8_t tos; // 8位服务类型
	
	u_int16_t tot_len; //16位总长度
	u_int16_t id;//16位标识
	u_int16_t frag_off ; //
	
	u_int8_t ttl ;// 8位生时间
	u_int8_t protocol ;// 8位协议
	u_int16_t check ;//16位校验和
	u_int31_t saddr ;
	u_int31_t daddr ;
};

// UDP头部
struct udphdr{
	u_int16_t source ;
	u_int16_t dest ;
	u_int16_t len ;
	u_int16_t check ;
};