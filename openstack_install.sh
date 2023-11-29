#/bin/bash
host_name="controller"
host_ip="192.168.26.153"
mariadb_root_pass="1"
#外部网卡设备名称
provider_net="ens36"
#外部网络网段
ext_net=192.168.100.0
#外部网络掩码位
ext_mask=24
#外部网络网关
ext_gateway=192.168.100.1
#外部网络DNS服务器
dns_server=8.8.8.8
#rabbit_pass="RABBIT_PASS"
#keystone_pass="ADMIN_PASS"
function showMsg(){
   echo -e "\033[32m$1\033[0m";
}
function showErr(){
   echo -e "\033[31m$1\033[0m";
}
function pre(){
   showMsg "配置主机名以及hosts解析"
   hostnamectl set-hostname $host_name
   echo "$host_ip $host_name" > /etc/hosts
}
function apt_source(){
    showMsg "配置Apt源"
    cp -rfv /etc/apt/sources.list /etc/apt/sources.list.back
    cat /etc/apt/sources.list.back | sed 's/cn.archive.ubuntu.com/mirrors.163.com/' > /etc/apt/sources.list
    showMsg "更新Apt源"
    apt update
    showMsg "升级软件包"
    apt upgrade -y
    if [ $? -eq 0 ];then
        showMsg "添加openstack zed源"
        add-apt-repository cloud-archive:zed -y
    else
        apt-get update --fix-missing
        dpkg --configure -a
        apt-get install -f
    fi
}
function mariadb(){
    showMsg "安装Mariadb数据库"
    apt install mariadb-server python3-pymysql -y
    showMsg "配置数据库"
    cat > /etc/mysql/mariadb.conf.d/99-openstack.cnf <<EOF
[mysqld]
bind-address = $host_ip
default-storage-engine = innodb
innodb_file_per_table = on
max_connections = 4096
collation-server = utf8_general_ci
character-set-server = utf8
EOF
}
function mariadb_service(){
    showMsg "重启数据库服务"
    service mysql restart
    showMsg "初始化数据库"
    apt install expect -y &> /dev/null
    expect -c "
    spawn mysql_secure_installation
    expect \"none):\"
    send \"\r\"
    expect \"authentication\"
    send \"n\r\"
    expect \"password?\"
    send \"y\r\"
    expect \"password:\"
    send \"${mariadb_root_pass}\r\"
    expect \"password:\"
    send \"${mariadb_root_pass}\r\"
    expect \"users?\"
    send \"y\r\"
    expect \"remotely?\"
    send \"n\r\"
    expect \"it?\"
    send \"y\r\"
    expect \"now?\"
    send \"y\r\"
    expect eof
    "
}
function rabbitmq_service(){
    showMsg "安装Rabbitmq-server"
    apt install rabbitmq-server -y
    rabbitmqctl add_user openstack RABBIT_PASS
    rabbitmqctl set_permissions openstack ".*" ".*" ".*"
}
function memcached_service(){
    showMsg "安装Memcached"
    apt install memcached python3-memcache -y
    showMsg "配置Memcached"
    sed -i "s/127.0.0.1/${host_ip}/" /etc/memcached.conf
}
function etcd_service(){
    showMsg "安装Etcd"
    apt install etcd -y
    cat > /etc/default/etcd <<EOF
ETCD_NAME="$host_name"
ETCD_DATA_DIR="/var/lib/etcd"
ETCD_INITIAL_CLUSTER_STATE="new"
ETCD_INITIAL_CLUSTER_TOKEN="etcd-cluster-01"
ETCD_INITIAL_CLUSTER="$host_name=http://$host_ip:2380"
ETCD_INITIAL_ADVERTISE_PEER_URLS="http://$host_ip:2380"
ETCD_ADVERTISE_CLIENT_URLS="http://$host_ip:2379"
ETCD_LISTEN_PEER_URLS="http://$host_ip:2380"
ETCD_LISTEN_CLIENT_URLS="http://$host_ip:2379"
EOF
    systemctl enable etcd
    systemctl restart etcd
}
function keystone(){
    showMsg "配置Keystone数据库"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE keystone;"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON keystone.* TO 'keystone'@'localhost' IDENTIFIED BY 'KEYSTONE_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON keystone.* TO 'keystone'@'%' IDENTIFIED BY 'KEYSTONE_DBPASS';"
    showMsg "安装Keystone软件包"
    apt install keystone  python3-openstackclient crudini -y
    cp /etc/keystone/keystone.conf /etc/keystone/keystone.conf.back
    cat /etc/keystone/keystone.conf.back | grep -Ev "^$|^#" > /etc/keystone/keystone.conf
    crudini --set /etc/keystone/keystone.conf database connection mysql+pymysql://keystone:KEYSTONE_DBPASS@$host_name/keystone
    crudini --set /etc/keystone/keystone.conf token provider fernet
    showMsg "同步数据库"
    su -s /bin/sh -c "keystone-manage db_sync" keystone
    keystone-manage fernet_setup --keystone-user keystone --keystone-group keystone
    keystone-manage credential_setup --keystone-user keystone --keystone-group keystone
    showMsg "引导服务"
    keystone-manage bootstrap --bootstrap-password ADMIN_PASS --bootstrap-admin-url http://$host_name:5000/v3/ --bootstrap-internal-url http://$host_name:5000/v3/ --bootstrap-public-url http://$host_name:5000/v3/ --bootstrap-region-id RegionOne
    echo "ServerName $host_name" >> /etc/apache2/apache2.conf
    cat > admin-openrc.sh <<EOF
export OS_USERNAME=admin
export OS_PASSWORD=ADMIN_PASS
export OS_PROJECT_NAME=admin
export OS_USER_DOMAIN_NAME=Default
export OS_PROJECT_DOMAIN_NAME=Default
export OS_AUTH_URL=http://$host_name:5000/v3
export OS_IDENTITY_API_VERSION=3
EOF
    showMsg "keystone测试"
    source admin-openrc.sh
    openstack token issue
    openstack project create --domain default --description "Service Project" service
}
function glance_service(){
    showMsg "配置Glance数据库"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE glance;"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON glance.* TO 'glance'@'localhost' IDENTIFIED BY 'GLANCE_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON glance.* TO 'glance'@'%'  IDENTIFIED BY 'GLANCE_DBPASS';"
    showMsg "创建Glance用户"
    source admin-openrc.sh
    openstack user create --domain default --password GLANCE_PASS glance
    openstack role add --project service --user glance admin
    showMsg "创建Glance服务"
    openstack service create --name glance --description "OpenStack Image" image
    showMsg "创建Glance端点"
    openstack endpoint create --region RegionOne  image public http://$host_name:9292
    openstack endpoint create --region RegionOne image internal http://$host_name:9292
    openstack endpoint create --region RegionOne image admin http://$host_name:9292
    showMsg "安装Glance软件包"
    apt install glance -y
    showMsg "修改Glance配置文件"
    cp /etc/glance/glance-api.conf /etc/glance/glance-api.conf.back
    cat /etc/glance/glance-api.conf.back | grep -Ev "^$|^#" > /etc/glance/glance-api.conf
    crudini --set /etc/glance/glance-api.conf database use_keystone_quotas True
    crudini --set /etc/glance/glance-api.conf database connection mysql+pymysql://glance:GLANCE_DBPASS@$host_name/glance
    crudini --set /etc/glance/glance-api.conf keystone_authtoken www_authenticate_uri http://$host_name:5000
    crudini --set /etc/glance/glance-api.conf keystone_authtoken auth_url http://$host_name:5000
    crudini --set /etc/glance/glance-api.conf keystone_authtoken memcached_servers $host_name:11211
    crudini --set /etc/glance/glance-api.conf keystone_authtoken auth_type password
    crudini --set /etc/glance/glance-api.conf keystone_authtoken project_domain_name Default
    crudini --set /etc/glance/glance-api.conf keystone_authtoken user_domain_name Default
    crudini --set /etc/glance/glance-api.conf keystone_authtoken project_name service
    crudini --set /etc/glance/glance-api.conf keystone_authtoken username glance 
    crudini --set /etc/glance/glance-api.conf keystone_authtoken password GLANCE_PASS
    crudini --set /etc/glance/glance-api.conf paste_deploy flavor keystone
    crudini --set /etc/glance/glance-api.conf glance_store stores file,http
    crudini --set /etc/glance/glance-api.conf glance_store default_store file
    crudini --set /etc/glance/glance-api.conf glance_store filesystem_store_datadir /var/lib/glance/images/
    showMsg "同步Glance数据库"
    su -s /bin/sh -c "glance-manage db_sync" glance
    showMsg "重启Glance服务"
    service glance-api restart
    showMsg "测试Glance"
    source admin-openrc.sh
    sleep 5 && openstack image list
}
function placement_service(){
    showMsg "配置Placement数据库"
    mysql -uroot -p"$mariadb_root_pass" -e " CREATE DATABASE placement;"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON placement.* TO 'placement'@'localhost' IDENTIFIED BY 'PLACEMENT_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON placement.* TO 'placement'@'%' IDENTIFIED BY 'PLACEMENT_DBPASS';"
    showMsg "创建Placement用户"
    source admin-openrc.sh
    openstack user create --domain default --password PLACEMENT_PASS placement
    openstack role add --project service --user placement admin
    showMsg "创建Placement服务"
    openstack service create --name placement --description "Placement API" placement
    showMsg "创建Placement端点"
    openstack endpoint create --region RegionOne placement public http://$host_name:8778
    openstack endpoint create --region RegionOne placement internal http://$host_name:8778
    openstack endpoint create --region RegionOne placement admin http://$host_name:8778
    showMsg "安装Placement软件包"
    apt install placement-api -y
    showMsg "配置Placement"
    cp /etc/placement/placement.conf /etc/placement/placement.conf.back
    cat /etc/placement/placement.conf.back | grep -Ev "^$|^#" > /etc/placement/placement.conf
    crudini --set /etc/placement/placement.conf placement_database connection mysql+pymysql://placement:PLACEMENT_DBPASS@$host_name/placement
    crudini --set /etc/placement/placement.conf api auth_strategy keystone
    crudini --set /etc/placement/placement.conf keystone_authtoken auth_url
    crudini --set /etc/placement/placement.conf keystone_authtoken auth_url http://$host_name:5000/v3
    crudini --set /etc/placement/placement.conf keystone_authtoken memcached_servers $host_name:11211
    crudini --set /etc/placement/placement.conf keystone_authtoken auth_type password
    crudini --set /etc/placement/placement.conf keystone_authtoken project_domain_name Default
    crudini --set /etc/placement/placement.conf keystone_authtoken user_domain_name Default
    crudini --set /etc/placement/placement.conf keystone_authtoken project_name service
    crudini --set /etc/placement/placement.conf keystone_authtoken username placement
    crudini --set /etc/placement/placement.conf keystone_authtoken password PLACEMENT_PASS
    showMsg "同步placement数据库"
    su -s /bin/sh -c "placement-manage db sync" placement
    service apache2 restart && sleep 10
    showMsg "服务检查"
    placement-status upgrade check
}
function nova_service(){
    showMsg "配置Nova数据库"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE nova_api;"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE nova;"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE nova_cell0;"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova_api.* TO 'nova'@'localhost' IDENTIFIED BY 'NOVA_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova_api.* TO 'nova'@'%' IDENTIFIED BY 'NOVA_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova.* TO 'nova'@'localhost' IDENTIFIED BY 'NOVA_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova.* TO 'nova'@'%' IDENTIFIED BY 'NOVA_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova_cell0.* TO 'nova'@'localhost' IDENTIFIED BY 'NOVA_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON nova_cell0.* TO 'nova'@'%' IDENTIFIED BY 'NOVA_DBPASS';"
    showMsg "创建Nova用户"
    source admin-openrc.sh
    openstack user create --domain default --password NOVA_PASS nova
    openstack role add --project service --user nova admin
    showMsg "创建Nova服务"
    openstack service create --name nova --description "OpenStack Compute" compute
    showMsg "创建Nova服务端点"
    openstack endpoint create --region RegionOne compute public http://$host_name:8774/v2.1
    openstack endpoint create --region RegionOne compute internal http://$host_name:8774/v2.1
    openstack endpoint create --region RegionOne compute admin http://$host_name:8774/v2.1
    showMsg "安装Nova软件包"
    apt install nova-api nova-conductor nova-novncproxy nova-scheduler nova-compute libvirt-daemon-driver-lxc qemu -y
    showMsg "修改Nova配置文件"
    cp /etc/nova/nova.conf /etc/nova/nova.conf.back
    cat /etc/nova/nova.conf.back | grep -Ev "^$|^#" > /etc/nova/nova.conf
    #打包错误 
    #sed -i '/log_dir/d' /etc/nova/nova.conf 
    crudini --set /etc/nova/nova.conf api_database my_ip "$host_ip"
    crudini --set /etc/nova/nova.conf api_database connection mysql+pymysql://nova:NOVA_DBPASS@$host_name/nova_api
    crudini --set /etc/nova/nova.conf database connection mysql+pymysql://nova:NOVA_DBPASS@$host_name/nova
    crudini --set /etc/nova/nova.conf DEFAULT transport_url rabbit://openstack:RABBIT_PASS@$host_name:5672/
    crudini --set /etc/nova/nova.conf api auth_strategy keystone
    crudini --set /etc/nova/nova.conf keystone_authtoken www_authenticate_uri  http://$host_name:5000/
    crudini --set /etc/nova/nova.conf keystone_authtoken auth_url http://$host_name:5000/
    crudini --set /etc/nova/nova.conf keystone_authtoken memcached_servers $host_name:11211
    crudini --set /etc/nova/nova.conf keystone_authtoken auth_type password
    crudini --set /etc/nova/nova.conf keystone_authtoken project_domain_name Default
    crudini --set /etc/nova/nova.conf keystone_authtoken user_domain_name Default
    crudini --set /etc/nova/nova.conf keystone_authtoken project_name service
    crudini --set /etc/nova/nova.conf keystone_authtoken username nova
    crudini --set /etc/nova/nova.conf keystone_authtoken password NOVA_PASS
    crudini --set /etc/nova/nova.conf libvirt virt_type kvm
    crudini --set /etc/nova/nova.conf scheduler discover_hosts_in_cells_interval 300
    crudini --set /etc/nova/nova.conf vnc enabled true
    crudini --set /etc/nova/nova.conf vnc server_listen '$my_ip'
    crudini --set /etc/nova/nova.conf vnc server_proxyclient_address '$my_ip'
    crudini --set /etc/nova/nova.conf vnc novncproxy_base_url http://"$host_ip":6080/vnc_auto.html
    crudini --set /etc/nova/nova.conf glance api_servers  http://$host_name:9292
    crudini --set /etc/nova/nova.conf oslo_concurrency lock_path /var/lib/nova/tmp
    crudini --set /etc/nova/nova.conf placement region_name RegionOne
    crudini --set /etc/nova/nova.conf placement project_domain_name Default
    crudini --set /etc/nova/nova.conf placement project_name service
    crudini --set /etc/nova/nova.conf placement auth_type password
    crudini --set /etc/nova/nova.conf placement user_domain_name Default
    crudini --set /etc/nova/nova.conf placement auth_url http://$host_name:5000/v3
    crudini --set /etc/nova/nova.conf placement username placement
    crudini --set /etc/nova/nova.conf placement password PLACEMENT_PASS
    crudini --set /etc/nova/nova.conf neutron auth_url http://$host_name:5000
    crudini --set /etc/nova/nova.conf neutron auth_type password
    crudini --set /etc/nova/nova.conf neutron project_domain_name default
    crudini --set /etc/nova/nova.conf neutron user_domain_name default
    crudini --set /etc/nova/nova.conf neutron region_name RegionOne
    crudini --set /etc/nova/nova.conf neutron project_name service
    crudini --set /etc/nova/nova.conf neutron username neutron
    crudini --set /etc/nova/nova.conf neutron password NEUTRON_PASS
    crudini --set /etc/nova/nova.conf neutron service_metadata_proxy true
    crudini --set /etc/nova/nova.conf neutron metadata_proxy_shared_secret METADATA_SECRET
    showMsg "同步Nova数据库"
    su -s /bin/sh -c "nova-manage api_db sync" nova
    su -s /bin/sh -c "nova-manage cell_v2 map_cell0" nova
    su -s /bin/sh -c "nova-manage cell_v2 create_cell --name=cell1 --verbose" nova
    su -s /bin/sh -c "nova-manage db sync" nova
    showMsg "注册检查"
    su -s /bin/sh -c "nova-manage cell_v2 list_cells" nova
    showMsg "重启Nova服务"
    service nova-api restart
    service nova-scheduler restart
    service nova-conductor restart
    service nova-novncproxy restart
    showMsg "发现计算节点"
    openstack compute service list --service nova-compute
    su -s /bin/sh -c "nova-manage cell_v2 discover_hosts --verbose" nova
}
function neutron_service(){
    showMsg "配置Neutron数据库"
    mysql -uroot -p"$mariadb_root_pass" -e "CREATE DATABASE neutron;"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON neutron.* TO 'neutron'@'localhost' IDENTIFIED BY 'NEUTRON_DBPASS';"
    mysql -uroot -p"$mariadb_root_pass" -e "GRANT ALL PRIVILEGES ON neutron.* TO 'neutron'@'%' IDENTIFIED BY 'NEUTRON_DBPASS';"
    showMsg "添加Neutron用户"
    source admin-openrc.sh
    openstack user create --domain default --password NEUTRON_PASS neutron
    openstack role add --project service --user neutron admin
    showMsg "创建Neutron服务"
    openstack service create --name neutron --description "OpenStack Networking" network
    showMsg "创建Neutron服务端点"
    openstack endpoint create --region RegionOne network public http://$host_name:9696
    openstack endpoint create --region RegionOne network internal http://$host_name:9696
    openstack endpoint create --region RegionOne network admin http://$host_name:9696
    showMsg "安装Neutron软件包"
    apt install neutron-server neutron-plugin-ml2 neutron-linuxbridge-agent neutron-l3-agent neutron-dhcp-agent neutron-metadata-agent -y
    showMsg "编辑Neutron配置文件"
    cp /etc/neutron/neutron.conf /etc/neutron/neutron.conf.back
    cat /etc/neutron/neutron.conf.back | grep -Ev "^$|^#" > /etc/neutron/neutron.conf
    crudini --set /etc/neutron/neutron.conf database connection mysql+pymysql://neutron:NEUTRON_DBPASS@$host_name/neutron
    crudini --set /etc/neutron/neutron.conf DEFAULT core_plugin ml2
    crudini --set /etc/neutron/neutron.conf DEFAULT service_plugins router
    crudini --set /etc/neutron/neutron.conf DEFAULT transport_url rabbit://openstack:RABBIT_PASS@$host_name
    crudini --set /etc/neutron/neutron.conf DEFAULT auth_strategy keystone
    crudini --set /etc/neutron/neutron.conf DEFAULT notify_nova_on_port_status_changes true
    crudini --set /etc/neutron/neutron.conf DEFAULT notify_nova_on_port_data_changes true
    crudini --set /etc/neutron/neutron.conf keystone_authtoken www_authenticate_uri http://$host_name:5000
    crudini --set /etc/neutron/neutron.conf keystone_authtoken auth_url http://$host_name:5000
    crudini --set /etc/neutron/neutron.conf keystone_authtoken memcached_servers $host_name:11211
    crudini --set /etc/neutron/neutron.conf keystone_authtoken auth_type password
    crudini --set /etc/neutron/neutron.conf keystone_authtoken project_domain_name default
    crudini --set /etc/neutron/neutron.conf keystone_authtoken user_domain_name default
    crudini --set /etc/neutron/neutron.conf keystone_authtoken project_name service
    crudini --set /etc/neutron/neutron.conf keystone_authtoken username neutron
    crudini --set /etc/neutron/neutron.conf keystone_authtoken password NEUTRON_PASS
    crudini --set /etc/neutron/neutron.conf experimental linuxbridge true
    crudini --set /etc/neutron/neutron.conf nova auth_url http://$host_name:5000
    crudini --set /etc/neutron/neutron.conf nova auth_type password
    crudini --set /etc/neutron/neutron.conf nova project_domain_name default
    crudini --set /etc/neutron/neutron.conf nova user_domain_name default
    crudini --set /etc/neutron/neutron.conf nova region_name RegionOne
    crudini --set /etc/neutron/neutron.conf nova project_name service
    crudini --set /etc/neutron/neutron.conf nova username nova
    crudini --set /etc/neutron/neutron.conf nova password NOVA_PASS
    crudini --set /etc/neutron/neutron.conf oslo_concurrency lock_path /var/lib/neutron/tmp
    showMsg "编辑二层配置文件"
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini ml2 type_drivers flat,vlan,vxlan
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini ml2 tenant_network_types vlan
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini ml2 mechanism_drivers linuxbridge,l2population
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini ml2 extension_drivers port_security
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini ml2_type_flat flat_networks provider
    crudini --set /etc/neutron/plugins/ml2/ml2_conf.ini securitygroup enable_ipset true
    showMsg "编辑LinuxBridge配置文件"
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini linux_bridge physical_interface_mappings provider:$provider_net
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini vxlan enable_vxlan false
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini vxlan local_ip $host_ip
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini vxlan l2_population true
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini securitygroup enable_security_group true
    crudini --set /etc/neutron/plugins/ml2/linuxbridge_agent.ini firewall_driver neutron.agent.linux.iptables_firewall.IptablesFirewallDriver
    showMsg "编辑Dhcp代理服务配置文件"
    crudini --set /etc/neutron/dhcp_agent.ini DEFAULT interface_driver linuxbridge
    crudini --set /etc/neutron/dhcp_agent.ini DEFAULT dhcp_driver neutron.agent.linux.dhcp.Dnsmasq
    crudini --set /etc/neutron/dhcp_agent.ini DEFAULT enable_isolated_metadata true
    showMsg "编辑三层配置文件"
    crudini --set /etc/neutron/l3_agent.ini DEFAULT interface_driver linuxbridge
    showMsg "配置元数据代理"
    crudini --set /etc/neutron/metadata_agent.ini DEFAULT nova_metadata_host $host_name
    crudini --set /etc/neutron/metadata_agent.ini DEFAULT metadata_proxy_shared_secret = METADATA_SECRET
    showMsg "同步Neutron数据库"
    su -s /bin/sh -c "neutron-db-manage --config-file /etc/neutron/neutron.conf --config-file /etc/neutron/plugins/ml2/ml2_conf.ini upgrade head" neutron
    showMsg "配置内核转发"
    echo 'net.bridge.bridge-nf-call-iptables = 1' >> /etc/sysctl.conf
    echo 'net.bridge.bridge-nf-call-ip6tables = 1' >> /etc/sysctl.conf
    sysctl -p
    showMsg "重启Nova,Neutron相关服务"
    service nova-compute restart
    service neutron-linuxbridge-agent restart
    systemctl restart nova-api neutron-server neutron-linuxbridge-agent neutron-dhcp-agent neutron-metadata-agent neutron-l3-agent
    showMsg "测试Neutron"
    sleep 10 && openstack network agent list
}
function horizon_service(){
    showMsg "安装dashboard"
    apt install openstack-dashboard -y
    showMsg "配置dashboard"
    sed -i 's@OPENSTACK_HOST = "127.0.0.1"@OPENSTACK_HOST = "'${host_ip}'"@' /etc/openstack-dashboard/local_settings.py
    sed -i 's@^#SESSION_ENGINE@SESSION_ENGINE@' /etc/openstack-dashboard/local_settings.py
    sed -i 's@.signed_cookies@.cache@' /etc/openstack-dashboard/local_settings.py
    sed -i 's@127.0.0.1:11211@'${host_name}':11211@' /etc/openstack-dashboard/local_settings.py
    sed -i 's@^OPENSTACK_KEYSTONE_URL@#OPENSTACK_KEYSTONE_URL@' /etc/openstack-dashboard/local_settings.py
    sed -i '/#OPENSTACK_KEYSTONE_URL/a\OPENSTACK_KEYSTONE_URL = "http://%s:5000/identity/v3" % OPENSTACK_HOST' /etc/openstack-dashboard/local_settings.py
    sed -i 's@UTC@Asia/Shanghai@' /etc/openstack-dashboard/local_settings.py
    echo 'OPENSTACK_KEYSTONE_MULTIDOMAIN_SUPPORT = True' >> /etc/openstack-dashboard/local_settings.py
    cat >> /etc/openstack-dashboard/local_settings.py <<EOF
OPENSTACK_API_VERSIONS = {
    "identity": 3,
    "image": 2,
    "volume": 3,
}
EOF
    echo 'OPENSTACK_KEYSTONE_DEFAULT_DOMAIN = "Default"' >> /etc/openstack-dashboard/local_settings.py
    echo 'OPENSTACK_KEYSTONE_DEFAULT_ROLE = "user"' >> /etc/openstack-dashboard/local_settings.py
    showMsg "重载apache2配置"
    systemctl reload apache2
    sleep 5 && systemctl restart apache2
    source admin-openrc.sh
    showMsg "创建安全组规则 TCP/UDP/ICMP"
    openstack security group rule create default  --protocol icmp --ingress --remote-ip 0.0.0.0/0
    openstack security group rule create default  --protocol icmp --egress --remote-ip 0.0.0.0/0
    openstack security group rule create default  --protocol tcp --ingress --remote-ip 0.0.0.0/0
    openstack security group rule create default  --protocol tcp --egress --remote-ip 0.0.0.0/0
    openstack security group rule create default  --protocol udp --ingress --remote-ip 0.0.0.0/0
    openstack security group rule create default  --protocol udp --egress --remote-ip 0.0.0.0/0
    showMsg "创建外部网络"
    openstack network create ext-net --external --provider-network-type flat --provider-physical-network provider --share
    openstack subnet create ext-subnet --subnet-range $ext_net/$ext_mask --gateway $ext_gateway --dns-nameserver $dns_server  --dhcp --network ext-net
    for i in {1..10}
    do
        sleep 1 && showMsg "等待服务启动...$i"
    done
    showMsg "访问Horizon http://$host_ip/horizon"
    showMsg "用户名: admin"
    showMsg "密码: ADMIN_PASS"
    showMsg "域: Default"
}
function service_check(){
    showMsg "Nova服务检查"
nova_svc=(
nova-api
nova-compute
nova-conductor
nova-novncproxy
nova-scheduler
)
for i in ${nova_svc[@]}
do
    systemctl status $i &> /dev/null
    if [ $? -eq 0 ]
    then
        showMsg "服务 $i 正常"
    else
        showErr "服务 $i 存在故障请检查日志 /var/log/$i.log"
        showMsg "尝试重启服务 $i"
        systemctl restart $i && sleep 5
        showMsg "请再次检查"
    fi
done
}
function check_update(){
    kernel_n=`cat /boot/grub/grub.cfg | grep vmlinuz | head -n 1 | awk '{print $2}' |awk -Fvmlinuz- '{print $2}'`
    uname -r | grep -E "$kernel_n"
    if [ $? = 0 ];then  
        mariadb
        mariadb_service
        rabbitmq_service
        memcached_service
        etcd_service
        keystone
        glance_service
        placement_service
        nova_service
        neutron_service
        horizon_service
    else
        showErr "请重启计算机后再次运行此脚本"
    fi
}
echo "*******************************************************************************"
echo "************************* Openstack Zed ***************************************"
echo "*******************************************************************************"
echo "********************** 0. 初 始 化 安 装 环 境 ********************************"
echo "********************** 1. 一 键 部 署 安 装 ***********************************"
echo "********************** 2. 服 务 检 查 *****************************************"
echo "*******************************************************************************"
echo "*************************Openstack Deploy Shell********************************"
echo "*******************************************************************************"
echo "*******************************************************************************"
read -p "请输入数字:" chose
if [ -z $chose ];then     
  showErr "请输入数字"
  exit
elif [ $chose = 0 ]
then
  pre
  apt_source
elif [ $chose = 1 ]
then
  check_update
elif [ $chose = 2 ]
then
  service_check
else
  exit
fi
