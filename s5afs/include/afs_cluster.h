#ifndef afs_cluster_h__
#define afs_cluster_h__
#include <stdint.h>
#include <uuid/uuid.h>

#include "s5_utils.h"

//node state 
#define NS_OK "OK"
#define NS_WARN "WARN"
#define NS_ERROR	"ERROR"

//TRAY state
#define TS_OK "OK"
#define TS_WARN "WARN"
#define TS_ERROR	"ERROR"

/**
 * ������zookeeper�����ӣ�
 * @param zk_ip_port zookeeper�ļ�ȺIP��ַ��afs������������ļ���
 *    [zookeeper]
 *    ip=192.168.0.253:2181,localhost:2181
 * ���ֻ�á�������������У�zk_ip_port��������"192.168.0.253:2181,localhost:2181"
 * @return 0��ʾ�ɹ���
 *         ������ʾʧ��, ����Ϊ-errno
 * 
 * @implementation
 *  �ú�����ʵ�֣�����
 *            static zhandle_t* zookeeper_handler;
 *            zookeeper_handler = createClient(zk_ip_port, &ctx);
 * �ɹ�����zookeeper���Ӻ�zookeeper_handler��Ϊ��̬����������������APIʵ���л�ʹ�øñ���
 */
int init_cluster(const char* zk_ip_port);

/**
 * ��zookeeperע��ڵ㣬ע��Ĺ��̾�����zookeeper��/s5/stores�ڵ����棬�������µĽڵ�ṹ��
 *         /s5/stores/
 *               |
 *               +<mngt_ip>   #store�ڵ�Ĺ���IP, ����ҲΪ����IP
 *                   |
 *                   +state  #״̬�� ����Ϊ��ERROR, WARN, OK
 *                   +alive  #EPHEMERAL���ͣ���ʾ��store�ڵ��Ƿ����ߣ�alive���ھͱ�ʾ����,����Ϊ��
 *                   +trays
 * @remark
 * register_store���Ӵ���sate�� alive�ڵ㣬��Ҫ�ڽڵ��ʼ����Ϻ󣬵���set_store_node_state���������˶��ڵ㡣
 * @return 0 on success, negative value on error. On error, an error message is logged.
 * @retval ZNONODE the parent node does not exist.
 * @retval ZNOAUTH the client does not have permission.
 * @retval ZNOCHILDRENFOREPHEMERALS cannot create children of ephemeral nodes.
 * @retval ZBADARGUMENTS - invalid input parameters
 * @retval ZINVALIDSTATE - zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE
 * @retval ZMARSHALLINGERROR - failed to marshall a request; possibly, out of memory
 */
int register_store_node(const char* mngt_ip);

/**
 * ��zookeeperע��һ��tray��ע��Ĺ��̾�����zookeeper��/s5/stores/<mngt_ip>trays�¶�Ӧ�洢ϵ�ڵ����棬�������µĽڵ�ṹ��
 * /s5/stores/<mngt_ip>/trays
 *						|
 *						+ <UUID>  #tray ��UUID��Ϊ�������
 *							|
 *							+devname  #��tray��device name
 *							+capacity #����
 *							+state    #��tray��״̬, ����Ϊ:ERROR, WARN, OK
 *							+online   # EPHEMERAL���ͣ���tray�Ƿ����ߣ�online�ڵ���ھͱ�ʾ����,����Ϊ��
 * �ú������Ե��ö�Σ�ÿ��ע��һ��tray
 * register_tray���Ӵ���sate�� online�ڵ㣬��Ҫ�ڽڵ��ʼ����Ϻ󣬵���set_tray_state���������˶��ڵ㡣
 * @return 0 on success, negative value on error. On error, an error message is logged.
 * @retval ZNONODE the parent node does not exist.
 * @retval ZNOAUTH the client does not have permission.
 * @retval ZNOCHILDRENFOREPHEMERALS cannot create children of ephemeral nodes.
 * @retval ZBADARGUMENTS - invalid input parameters
 * @retval ZINVALIDSTATE - zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE
 * @retval ZMARSHALLINGERROR - failed to marshall a request; possibly, out of memory
 */
int register_tray(const char* mngt_ip, const uuid_t uuid, const char* devname, int64_t capacity);

/**
 * set store node's state. create `state` and `alive` node on zookeeper, if not exists. 
 * @seealso register_store for tree structure of store node in zookeeper
 * @param mngt_ip  management IP of store node, use as ID of store node
 * @param state store node's state, value can NS_OK, NS_WARN, NS_ERROR
 * @param alive TRUE for alive, FALSE for not 
 * @return 0 on success, negative value on error. On error, an error message is logged.
 * @retval ZNONODE the parent node does not exist.
 * @retval ZNOAUTH the client does not have permission.
 * @retval ZNOCHILDRENFOREPHEMERALS cannot create children of ephemeral nodes.
 * @retval ZBADARGUMENTS - invalid input parameters
 * @retval ZINVALIDSTATE - zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE
 * @retval ZMARSHALLINGERROR - failed to marshall a request; possibly, out of memory
*/
int set_store_node_state(const char* mngt_ip, const char* state, BOOL alive);

/**
 * set tray state. create `state` and `online` node on zookeeper, if not exists. 
 * @seealso register_tray for tree structure of tray node in zookeeper.
 * @param mngt_ip IP of store node of the tray
 * @param tray_uuid uuid of tray to set
 * @param state, tray state, can be TS_OK, TS_WARN, TS_ERROR
 * @online TRUE for online, FALSE for not
 * @return 0 on success, negative value on error. On error, an error message is logged.
 * @retval ZNONODE the parent node does not exist.
 * @retval ZNOAUTH the client does not have permission.
 * @retval ZNOCHILDRENFOREPHEMERALS cannot create children of ephemeral nodes.
 * @retval ZBADARGUMENTS - invalid input parameters
 * @retval ZINVALIDSTATE - zhandle state is either ZOO_SESSION_EXPIRED_STATE or ZOO_AUTH_FAILED_STATE
 * @retval ZMARSHALLINGERROR - failed to marshall a request; possibly, out of memory
 */
int set_tray_state(const char* mngt_ip, const uuid_t tray_uuid, const char* state, BOOL online);

#endif // afs_cluster_h__
