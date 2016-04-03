#ifndef afs_cluster_h__
#define afs_cluster_h__

/**
 * ������zookeeper�����ӣ�
 * @param zk_ip_port zookeeper�ļ�ȺIP��ַ��afs������������ļ���
 *    [zookeeper]
 *    ip=192.168.0.253:2181,localhost:2181
 * ���ֻ�á�������������У�zk_ip_port��������"192.168.0.253:2181,localhost:2181"
 * @return 0��ʾ�ɹ���
 *         ������ʾʧ��
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
 */
int register_store(const char* mngt_ip);

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
 */
int register_tray(const char* mngt_ip, const char* uuid, const char* devname, int64_t capacity);

#endif // afs_cluster_h__
