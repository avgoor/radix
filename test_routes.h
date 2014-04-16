/* dvm@avgoor.ru */

#ifndef _TEST_ROUTES_H_
#define _TEST_ROUTES_H_

struct radix_node_M {
	short state;		/* Status of node */
#define	RNM_LOCKED		1
#define	RNM_FREE		2
#define	RNM_UNMAPPED	3
	int index;			/* */
	
	struct radix_node_M *next;	/* Link to next node */
	struct radix_node_M *prev;	/* Link to previous node */
	
	

};

struct rt_metrics_lite {
	u_long	rmx_mtu;	/* MTU for this path */
	u_long	rmx_expire;	/* lifetime for route, e.g. redirect */
	u_long	rmx_pksent;	/* packets sent using this route */
	u_long	rmx_weight;	/* absolute weight */ 
};


struct rtentry {
	struct	radix_node rt_nodes[2];	/* tree glue, and other values */
	/*
	 * XXX struct rtentry must begin with a struct radix_node (or two!)
	 * because the code does some casts of a 'struct radix_node *'
	 * to a 'struct rtentry *'
	 */
#define	rt_key(r)	(*((struct sockaddr **)(&(r)->rt_nodes->rn_key)))
#define	rt_mask(r)	(*((struct sockaddr **)(&(r)->rt_nodes->rn_mask)))
	struct	sockaddr *rt_gateway;	/* value */
	int	rt_flags;		/* up/down?, host/net */
	int	rt_refcnt;		/* # held references */
	struct	ifnet *rt_ifp;		/* the answer: interface to use */
	struct	ifaddr *rt_ifa;		/* the answer: interface address to use */
	struct	rt_metrics_lite rt_rmx;	/* metrics used by rx'ing protocols */
	u_int	rt_fibnum;		/* which FIB */

};

struct rt_metrics {
	u_long	rmx_locks;	/* Kernel must leave these values alone */
	u_long	rmx_mtu;	/* MTU for this path */
	u_long	rmx_hopcount;	/* max hops expected */
	u_long	rmx_expire;	/* lifetime for route, e.g. redirect */
	u_long	rmx_recvpipe;	/* inbound delay-bandwidth product */
	u_long	rmx_sendpipe;	/* outbound delay-bandwidth product */
	u_long	rmx_ssthresh;	/* outbound gateway buffer limit */
	u_long	rmx_rtt;	/* estimated round trip time */
	u_long	rmx_rttvar;	/* estimated rtt variance */
	u_long	rmx_pksent;	/* packets sent using this route */
	u_long	rmx_weight;	/* route weight */
	u_long	rmx_filler[3];	/* will be used for T/TCP later */
};

#define	rn_dupedkey	rn_u.rn_leaf.rn_Dupedkey
#define	rn_key		rn_u.rn_leaf.rn_Key
#define	rn_mask		rn_u.rn_leaf.rn_Mask
#define	rn_offset	rn_u.rn_node.rn_Off
#define	rn_left		rn_u.rn_node.rn_L
#define	rn_right	rn_u.rn_node.rn_R

#endif
