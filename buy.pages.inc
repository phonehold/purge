<?php


function base_form() {
    return theme('buy_html_form'); 
}
//添加IDC
function buy_add_idc(){
  $form['buy']=array(
  '#type' => 'fieldset',
  '#title'=> t('数据中心'),
  );
  $form['buy']['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写主机对应机房信息'),
    '#default_value' => t('普尔网络IDC'),
     '#required' => TRUE,
  );
/*
 $form['buy']['type'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写对应的线路类型'),
    '#description' => t('电信 联通 BGP 或双线 '),
    '#default_value' => t(''),
     '#required' => TRUE, 
  );
*/
$form['buy']['type'] = array(
    '#type' => 'select', 
    //'#title' => t('选择对应的线路类型'), 
    '#default_value' => t('电信'), 
    '#options' =>  drupal_map_assoc(array(
      1 => t('电信'),
      2 => t('联通'),
      3 => t('双线'),
      4 => t('BGP'),
      5 => t('移动'),
      6 => t('铁通'),
      7 => t('歌华'),
    )), 
    '#description' => t('选择对应的线路类型'),
  );

 $form['buy']['address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写对应IDC机房地址'),
    '#description' => t('北京市朝阳区酒仙桥东路1号M5楼三层'),
    '#default_value' => t(''),
     '#required' => TRUE, 
  );
  $form['buy']['contact'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写对应联系人信息'),
    '#description' => t('沈学伦'),
    '#default_value' => t(''),
     '#required' => TRUE, 
  );
   $form['buy']['phone'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写对应联系人手机或电话'),
    '#description' => t('18601044856'),
    '#default_value' => t(''),
     '#required' => TRUE, 
  );
   $form['buy']['status'] = array(
    '#type'          => 'textfield',
    '#description' => t('IDC是否启用'),
    '#default_value' => 1,
     '#required' => TRUE, 
  );
   $form['buy']['submit'] = array(
     '#type' => 'submit',
     '#value' => '提交',
   ); 

  return $form;
  
  }



function get_idc(){
$idc = db_query("SELECT name FROM buy_idc where status=1")->fetchAll();
return $idc;
 }

//添加表单
function buy_add() {
   $idc = db_query("SELECT name FROM buy_idc where status=1")->fetchAll();
   //print_r(array_count_values((array)$idc));exit;
//   print_r(((array)$idc));exit;
  foreach($idc as $value) {
    $idcs[] = $value->name;
  }
  if (empty($idcs)){
      $idcs=array(1=>t('没有添加线路类型'),);
      }
//print_r($idcs);exit;
  $form['buy'] = array(
    '#type'  => 'fieldset',
    '#title' => t('添加主机'),
  );
    $form['buy']['location'] = array(
    '#type'          => 'select',
    '#title'         => t('选择机房信息'),
    '#default_value' => t('普尔网络IDC'),
    '#options' =>drupal_map_assoc($idcs),
  ); 
  $form['buy']['ip'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写IP地址'),
     '#required' => TRUE, 
   // '#description'   => t('ip.'),
  );
  $form['buy']['port'] = array(
    '#type'          => 'textfield',
    '#title'         => t('填写主机端口'),
    '#default_value' => '80',
     '#required' => TRUE, 
  ); 
  $form['buy']['status'] = array(
    '#type'          => 'textfield',
    '#title'         => t('主机状态'),
    '#default_value' => '1',
   // '#description'   => t('status.'),
  );

  $form['buy']['submit'] = array(
    '#type' => 'submit',
    '#value' => '提交',
  ); 
  return $form;
}
// 刷新表单
function buy_purge(){
  $form['buy']=array(
   '#type'  => 'fieldset',
   '#title' => t('添加刷新URL'),
  );
  $form['buy']['url']=array(
  '#type'          => 'textfield',
  '#title'         => t('刷新url'),
  '#description' => t('例: http://www.360buy.com/index.html'),
  );

  $form['buy']['submit'] = array(
    '#type' => 'submit',
    '#value' => '提交',
  ); 
  return $form;

  }

function buy_purge_validate($form,&$form_state){
  $url=$form_state['values']['url'];
  //print $url;  
  }

function buy_purge_submit($form,$form_state){
$purl=trim($form_state['values']['url']);
$purl_tmp =parse_url($purl);
$path = $purl_tmp['path'];
$host = $purl_tmp['host'];
$scheme = $purl_tmp['scheme'];
//$count= db_query("SELECT COUNT(DISTINCT ip) FROM buy where status=1");
//$count1= db_query("SELECT COUNT(DISTINCT ip) as count FROM buy where status=1")->fetchAll();
$count1= db_query("SELECT COUNT(ip) as count FROM buy where status=1")->fetchAll();
//form_set_error('buy', t('%count ', array('%count' =>  $count['0']->count)));
$count= $count1['0']->count;
$ips = db_query("SELECT ip,port FROM buy where status=1")->fetchAll();

//foreach ($ips as $key => $value)
//{
// $ip[]=$value->ip;

//}

     // Set the blocking mode.
     $options = array(
       'blocking' => TRUE,
       'method' => 'PURGE',
       'headers'=>array(
       'User-Agent'=> 'Smart CDN Purge',
       'Host' => $host,
       ),
    );
    foreach ($ips as $value) {
      $url = $scheme.'://'.$value->ip . ':' . $value->port .$path;
      //echo "$url\n";
      //break;
           //httprl_request($scheme.'://'.$value .$path, $options);
           httprl_request($url, $options);
           }
           $request = httprl_send_request();
           // echo httprl_pr($request);
           //exit;
           //variable_set('final', serialize($request));
 
$body=serialize($request);
 $row = array(
    'body' => $body,
    'created' => time(),
  );
  db_insert('buy_purgelog')->fields($row)->execute();
    drupal_set_message(t('共有 @count 台主机刷新请求被提交',array('@count' => $count)));
  }

function buy_add_validate($form, &$form_state){
  $ip = $form_state['values']['ip'];
  $id = isset($form_state['values']['id']) ? $form_state['values']['id'] : '';
  $status = $form_state['values']['status'];
  if (!is_numeric($status)) {
    form_set_error('buy', t('状态必须为数值（0或1）'));
  }
  if (filter_var(trim($ip), FILTER_VALIDATE_IP, FILTER_FLAG_NO_RES_RANGE) == FALSE) {
    form_set_error('buy', t('ip格式不正确.'));
  }
  //$query = db_query("SELECT ip FROM {buy} WHERE ip like :ip AND id != :id", array(':ip' => $ip, ':id' => $id))->fetchObject();
  //if ($query &&  $query->ip == $ip) {
  //  form_set_error('buy', t('主机: %ip 已经存在', array('%ip' => $ip)));
  //}
}

function buy_add_submit($form, $form_state){
  $location = $form_state['values']['location'];
  $ip = $form_state['values']['ip'];
  $status = $form_state['values']['status'];
  $port = $form_state['values']['port'];
  //$id = db_next_id(db_query('SELECT MAX(id) FROM {buy}')->fetchField());
  $row = array(
    'location' => $location,
    'ip' => $ip,
    'port' => $port,
    'status' =>  $status,
    'time' => time(),
    //'id' => $id,
  );
  db_insert('buy')->fields($row)->execute();
  drupal_set_message(t('主机: %ip 添加成功', array('%ip' => $ip)));
}
function buy_add_idc_submit($form,$form_state){
 //print_r($form_state);exit;
  $name = $form_state['values']['name'];
  $address = $form_state['values']['address'];
  $type = $form_state['values']['type'];
  $contact = $form_state['values']['contact'];
  $phone = $form_state['values']['phone'];
  $status = $form_state['values']['status'];
  $row =array(
  'name' => $name,
  'type' => $type,
  'address' => $address,
  'contact' => $contact,
  'phone' => $phone,
  'status' => $status,
  'created' => time(),
  );
  db_insert('buy_idc')->fields($row)->execute();
  drupal_set_message(t('主机: %name 添加成功', array('%name' => $name)));
  }
//删除动作
function buy_del($id) {
  db_delete('buy')->condition('id', $id)->execute();  
  drupal_set_message(t('删除成功'));
  drupal_goto('buy');
}

//删除idc数据中心
function buy_delidc($id){
  //print $id;exit;
  db_delete('buy_idc')->condition('id', $id)->execute();
 drupal_set_message(t('删除成功'));
 drupal_goto('buy/listidc');
  }

//刷新日志列表

function ip2loc($ip){
  //$port = db_query("SELECT port FROM buy where status=1 and ip='$ip'")->fetchField();
  $loc = db_query("SELECT location FROM buy where ip='$ip'")->fetchField();
  //print_r($loc);exit;
  return $loc;
  }


function buy_purge_log(){
$header = array(
      '机房信息',
      '主机IP',
      'port', 
      '刷新URL', 
      '状态码',
      '刷新时间'=>array('data'=>t('刷新时间'),'field'=>'b.created','sort'=>'desc'),
    );
//$result=db_query("select body from buy_purgelog order by id desc limit 0,100 ")->fetchAll();
$query = db_select('buy_purgelog', 'b')->extend('PagerDefault');
$result = $query->orderBy('id', 'desc')
		->fields('b',array('body','created'))
       		->limit('10')
       		->execute()
		->fetchAll();
//print_r(unserialize($result));exit;
//$result = unserialize(variable_get('final'));
//分页
if (empty($result)){
$result ="还没有刷新记录";
$rows1 = array();
$rows1[] = array(array('data' => $result, 'colspan' =>'5', 'style' => 'text-align:center;'));
}
else{
//$result = pager_array($result, $limit = 10, $element = 0);
//print_r($result);exit;
//$rows = array();
foreach ($result as $rows) {
  $rowss = unserialize($rows->body);
  $time = $rows->created;
// print_r($row);exit;

if ($rowss) {
foreach($rowss as $row) {
	$ip = parse_url($row->url);
	$code = $row->code;
	//break;
}
}

//print_r($ip);exit;
//  $ip = parse_url($row->url);
  $scheme = $ip['scheme'];
  $path = $ip['path'];
  $port = $ip['port'];
  $host = $row->options;
  $host = $host['headers']['Host'];
  $url = $scheme.'://'.$host .$path;
  //$port = ip2port($ip['host']);
  $loc = ip2loc($ip['host']);
  $rows1[] = array('data' => array($loc,$ip['host'],$port, $url, $code,date('Y-m-d H:i:s', $time)));
    }
}

$form['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows1,
      );

$form['pager'] = array('#markup' => theme('pager'));
    return $form;
}















//编辑表单
function buy_edit($form, &$form_state, $id) {
 $form['buy'] = array(
    '#type'  => 'fieldset',
    '#title' => t('buy data edit'),
  );
  
   $form['buy']['location'] = array(
    '#type'          => 'textfield',
    '#title'         => t('机房信息'),
    '#default_value' => $id->location,
  );

  $form['buy']['ip'] = array(
    '#type'          => 'textfield',
    '#title'         => t('ip'),
    '#description'   => t('ip.'),
    '#default_value' => $id->ip,
  );
 
   $form['buy']['port'] = array(
    '#type'          => 'textfield',
    '#title'         => t('主机端口'),
    '#default_value' => $id->port,
  );
  $form['buy']['status'] = array(
    '#type'          => 'textfield',
    '#title'         => t('status'),
    '#default_value' => $id->status,
    '#description'   => t('status'),
  );


  $form['buy']['id'] = array(
    '#type' => 'value',
    '#value' => $id->id,
  ); 
  $form['buy']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    '#validate' => array('buy_add_validate'),
  ); 
  return $form;
}

function buy_edit_submit($form, $form_state){
  $location = $form_state['values']['location'];
  $ip = $form_state['values']['ip'];
  $port = $form_state['values']['port'];
  $status = $form_state['values']['status'];
  $id = $form_state['values']['id'];
  $row = array(
    'location' => $location,
    'ip' => $ip,
    'port' => $port,
    'status' =>  $status,
    'time' => time(),
    //'id' => $id,
  );
  db_update('buy')->fields($row)->condition('id', $id)->execute();
  drupal_set_message(t('数据: %ip 编辑成功', array('%ip' => $ip)));
  drupal_goto('buy');
}

//IDC编辑

function buy_editidc($form, &$form_state, $id) {
   $form['buy'] = array(
    '#type'  => 'fieldset',
    '#title' => t('IDC数据中心编辑'),
  );
   $form['buy']['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('机房信息'),
    '#default_value' => $id->name,
  );
  $form['buy']['type'] = array(
    '#type' => 'select',  
    '#default_value' => t('电信'), 
    '#options' =>  drupal_map_assoc(array(
      1 => t('电信'),
      2 => t('联通'),
      3 => t('双线'),
      4 => t('BGP'),
      5 => t('移动'),
      6 => t('铁通'),
      7 => t('歌华'),
    )), 
     '#default_value' => $id->type,
    '#description' => t('选择对应的线路类型'),
  );
   $form['buy']['address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('地址'),
    '#default_value' => $id->address,
  );
  $form['buy']['contact'] = array(
    '#type'          => 'textfield',
    '#title'         => t('联系人'),
    '#default_value' => $id->contact,
    '#description'   => t('contact'),
  );  

  $form['buy']['phone'] = array(
    '#type'          => 'textfield',
    '#title'         => t('手机'),
    '#default_value' => $id->phone,
    '#description'   => t('phone'),
  );  
  $form['buy']['status'] = array(
    '#type'          => 'textfield',
    '#title'         => t('状态'),
    '#default_value' => $id->status,
    '#description'   => t('status'),
  );  

  $form['buy']['id'] = array(
    '#type' => 'value',
    '#value' => $id->id,
  );  
  $form['buy']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
    //'#validate' => array('buy_add_validate'),
  ); 
  return $form;
}


function buy_editidc_submit($form, $form_state){
  $name = $form_state['values']['name'];
  $type = $form_state['values']['type'];
  $address = $form_state['values']['address'];
$contact= $form_state['values']['contact'];
$phone = $form_state['values']['phone'];

  $status = $form_state['values']['status'];
  $id = $form_state['values']['id'];
  $row = array(
    'name' => $name,
    'type' => $type,
    'address' => $address,
    'contact' => $contact,
    'phone' => $phone,
    'status' =>  $status,
    'created' => time(),
    //'id' => $id,
  );
  db_update('buy_idc')->fields($row)->condition('id', $id)->execute();
  drupal_set_message(t('数据: %name 编辑成功', array('%name' => $name)));
  drupal_goto('buy');
}




//IDC 列表
function buy_list_idc(){
  
  $header = array(
    'id' => t('ID'),
    'name' => t('机房信息'),
    'type' => array('data'=>t('类型'),'field'=>'u.type','sort'=> 'desc'),
    'address' => array('data'=>t('地址'),'field'=>'u.address','sort'=> 'asc'),
    'status' => array('data' => t('状态'), 'field' => 'u.status'),
    'created' => array('data' => t('提交时间'), 'field' => 'u.created', 'sort' => 'desc'),
    'operations' => array('data' => t('操作')),
  );
  $query = db_select('buy_idc', 'u');
  $query->condition('u.id', 0, '<>');
  $search_key = '';
  if (!empty($_SESSION['seach_key'])){
    $search_key = trim($_SESSION['seach_key']);
    $or = db_or()->condition('u.name', '%'. $search_key .'%', 'like')
                 ->condition('u.id', $search_key);
    $query->condition($or);
    unset($_SESSION['seach_key']);  //刷新后是否保留数据
  }
  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.id)');
  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('id', 'name','type','address','status', 'created'))
    ->limit(5)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('操作选项'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['options']['search_key'] = array(
    '#type' => 'textfield',
    '#title' => t('搜索'),
    '#maxlength' => 128,
    '#size' => 18,
    '#default_value' => $search_key,
  );
  $options = array();
  $form['options']['search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交查询'),
    //'#method' => 'POST',
    //'#submit' =>array('jjj'),
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array('请选择', '启用', '禁用', '删除'),
    '#default_value' => 'unblock',
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交'),
  );
  $destination = drupal_get_destination();
  $status = array(1 => t('启用'), 2=> t('禁用'), 0 => t('无状态'));
  $datas = array();
  foreach ($result as $datas) {
    $options[$datas->id] = array(
      'id' => $datas->id,
      'name' => $datas->name,
      'type' => $datas->type,
      'address' => $datas->address,
      'status' =>  $status[$datas->status],
      'created' =>  date('Y-m-d H:i:s', $datas->created),
      'operations' => l(t('编辑'), 'buy/edit/idc/' . $datas->id) . ' ' . l(t('删除'), 'buy/del/idc/' . $datas->id),
    );
  }
  $form['datas'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('没有数据'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
  }
//批量操作IDC 

function buy_list_idc_submit($form, &$form_state) {
  $status = $form_state['values']['operation'];
  $datas = array_filter($form_state['values']['datas']);
  foreach ($datas as $id => $value) {
      if ($value) {
        switch ($status) {
          case '1':
          $num_enable = db_update('buy_idc')
          ->fields(array(
             'status' => 1,
          ))
          ->condition('id', $id, '=')
          ->execute();
            break;
          case '2':
          $num_enable = db_update('buy_idc')
          ->fields(array(
             'status' => 2,
          ))
          ->condition('id', $id, '=')
          ->execute();
            break;
          case '3':
          db_delete('buy_idc')
          ->condition('id', $id, '=')
          ->execute();
           break;
        }
      }
  }
}



//数据列表页
function buy_list() {
  $header = array(
    'id' => t('ID'),
    'location' => t('机房信息'),
    'ip' => array('data'=>t('ip地址'),'field'=>'u.ip','sort'=> 'desc'),
    'port' => array('data'=>t('主机端口'),'field'=>'u.port','sort'=> 'asc'),
    'status' => array('data' => t('状态'), 'field' => 'u.status'),
    'time' => array('data' => t('提交时间'), 'field' => 'u.time', 'sort' => 'desc'),
    'operations' => array('data' => t('操作')),
  );
  $query = db_select('buy', 'u');
  $query->condition('u.id', 0, '<>');
  $search_key = '';
  if (!empty($_SESSION['seach_key'])){
    $search_key = trim($_SESSION['seach_key']);
    $or = db_or()->condition('u.ip', '%'. $search_key .'%', 'like')
                 ->condition('u.id', $search_key);
    $query->condition($or);
    unset($_SESSION['seach_key']);  //刷新后是否保留数据
  }
  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.id)');
  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('id', 'location','ip','port','status', 'time'))
    ->limit(5)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('操作选项'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['options']['search_key'] = array(
    '#type' => 'textfield',
    '#title' => t('搜索'),
    '#maxlength' => 128,
    '#size' => 18,
    '#default_value' => $search_key,
  );
  $options = array();
  $form['options']['search_submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交查询'),
    //'#method' => 'POST',
    //'#submit' =>array('jjj'),
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array('请选择', '启用', '禁用', '删除'),
    '#default_value' => 'unblock',
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('提交'),
  );
  $destination = drupal_get_destination();
  $status = array(1 => t('启用'), 2=> t('禁用'), 0 => t('无状态'));
  $datas = array();
  foreach ($result as $datas) {
    $options[$datas->id] = array(
      'id' => $datas->id,
      'location' => $datas->location,
      'ip' => $datas->ip,
      'port' => $datas->port,
      'status' =>  $status[$datas->status],
      'time' =>  date('Y-m-d H:i:s', $datas->time),
      'operations' => l(t('编辑'), 'buy/edit/' . $datas->id) . ' ' . l(t('删除'), 'buy/del/' . $datas->id),
    );
  }
  $form['datas'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('没有数据'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}

function buy_list_validate($form, &$form_state) {
  $form_state['values']['datas'] = array_filter($form_state['values']['datas']);
  if (count($form_state['values']['datas']) == 0 && $form_state['values']['operation'] != 0) {
    form_set_error('', t('请选择操作内容'));
  }
  if (empty($form_state['values']['search_key'])) {
    //form_set_error('', t('请输入查找的内容'));
  }
  $_SESSION['seach_key'] = $form_state['values']['search_key'];
}

//批量操作的动作
function buy_list_submit($form, &$form_state) {
  $status = $form_state['values']['operation'];
  $datas = array_filter($form_state['values']['datas']);
  foreach ($datas as $id => $value) {
      if ($value) {
        switch ($status) {
          case '1':
          $num_enable = db_update('buy')
          ->fields(array(
             'status' => 1,
          ))
          ->condition('id', $id, '=')
          ->execute();
            break;
          case '2':
          $num_enable = db_update('buy')
          ->fields(array(
             'status' => 2,
          ))
          ->condition('id', $id, '=')
          ->execute();
            break;
          case '3':
          db_delete('buy')
          ->condition('id', $id, '=')
          ->execute();
           break;
        }
      }
  }
}




function pager_array($array, $limit = 20, $element = 0) {
global $pager_page_array, $pager_total, $pager_total_items;
$page = isset($_GET['page']) ? $_GET['page'] : '';
$pager_page_array = explode(',', $page);
$pager_total_items[$element] = count($array);
$pager_total[$element] = ceil($pager_total_items[$element] / $limit);
$pager_page_array[$element] = max(0, min((int)$pager_page_array[$element], ((int)$pager_total[$element]) - 1));
return array_splice($array,$pager_page_array[$element]*$limit,$limit);
}








/*
function buy_operation($datas, $status) {
  // Filter out unchecked accounts.
  $datas = array_filter($form_state['values']['datas']);
  foreach ($datas as $id => $value) {
      if ($value) {
        switch ($status) {
          case '1':
          $num_enable = db_update('buy')
          ->fields(array(
             'status' => 1,
          ))
          ->condition('id', $id, '=')
          ->execute();
          break;
          case '2':
          $num_enable = db_update('buy')
          ->fields(array(
             'status' => 2,
          ))
          ->condition('id', $id, '=')
          ->execute();
          break;
          case '3':
           db_query('DELETE FROM {buy} WHERE id in (:id)', array(':id' => $id));
          break;
        }
      }
  }
  //$row = array(':status' => $status, ':id' => $datas);
  //db_query('UPDATE {buy} SET status = :status WHERE id in (:id)', array(':status' => $status, ':id' => $datas));
  //db_update('buy')->fields($row)->condition('id', '4,5,6,7' , 'in')->execute();
  drupal_set_message(t('操作成功'));
  drupal_goto('buy');
}


/**
* build the edit and clone form of zone
*/
/*
function buy_form($form, &$form_state, $formtype, $rows) {

  $form['data'] = array(
    '#type' => 'textfield',
    '#title' => t('数据'),
    '#required' => TRUE,
    '#default_value' => isset($rows->data) ? $rows->data : '',
    '#description' => t(''),
    '#size' => 40,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#validate' => array('buy_form_validate'),
    '#submit' => array('buy_form_submit'),
  );
  return $form;
}

function buy_form_validate($form, &$form_state) {
  $data = $form_state['values']['data'];
  $query = db_query("SELECT ip FROM {buy} WHERE data = :data", array(':data' => $data))->fetchObject();
  if ($query &&  $query->data == $data) {
    form_set_error('', t('数据: %data 已经存在', array('%data' => $data)));
  }

}

function buy_form_submit($form, &$form_state) {
  $id = db_next_id(db_query('SELECT MAX(id) FROM {buy}')->fetchField());
  $name = $form_state['values']['name'];
  $row = array(
    'name' => $name,
    'mail' =>  $form_state['values']['mail'],
    'created' => time(),
    'id' => $uid,
  );
  db_insert('buy')->fields($row)->execute();
  drupal_set_message(t('用户: %name 添加成功', array('%name' => $name)));
  drupal_goto ('user/' .$GLOBALS['user']->uid. '/buy' );
}



//添加域名的表单，暂时不用
function inputurl($form_state) {
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('请输入域名'),
    '#prefix' => '<div class="container-inline" style="text-align:center; width:500px;">',
    '#suffix' => '</div>',
  );
//  $form['#attributes']['onsubmit']="showMessage()";
  $form['options']['key'] = array(
    '#type' => 'textfield',
    '#title' => 'www.',
    '#default_value' => '',
    '#maxlength' => 128,
    '#size' => 30,
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('添加域名'),
    //'#submit' => array('audit_filter_form_submit'),
  );
  return $form;
}

function inputurl_validate($form, &$form_state) {
  $key = $form_state['values']['key'];
  if (!preg_match("/^[A-Za-z0-9]+\.[A-Za-z0-9]+[\/=\?%\-&_~`@[\]\':+!]*([^<>\"])*$/", $key)) {   
    form_error($form, t("请输入正确的网址"));
  }    
  //$form_state['values']['key']="bbb";   //过滤域名
  
   $query = db_select('cloud_domain', 'v');
      $query->addField('v', 'name');
      $taken = $query
        ->condition('name', $key)
        ->countQuery()
        ->execute()
        ->fetchField();
    if ($taken != '') {
        form_error($form, t("域名已经被输入过了，请检查"));
        
    }
        
}

function inputurl_submit($form,&$form_state){
  $key = $form_state['values']['key']; 
  db_insert('cloud_domain')
    ->fields(array(
      'name' => $key,
      'id' => $GLOBALS['user']->uid,
      'cid' => 0,
      'starttime' =>time(),
      ))
    ->execute();
  drupal_set_message($form_state['values']['key']."添加成功！");
}
*/
