<?php

/**
 * @file
 */
?>

<form action="<?php echo $_SERVER['PHP_SELF'];?>" method="POST" name="biaodan">
  <table width="691" border="1" cellspacing="1" cellpadding="1" summary="表格">
    <tr><td width="191">名字：</td>
    <td width="487"><input type="text" name="xingming" /></td></tr>
    <tr><td>姓别：</td><td><input name="xingbie" type="radio" value="男" checked="checked" />男
    <input name="xingbie" type="radio" value="女" />女</td></tr>
    <tr><td>个人介绍：</td>
      <td><textarea name="jieshao" cols="50" rows="5"></textarea></td></tr>
    <tr><td>出生日期：</td><td><input name="nian" type="text" value="1980" size="6" maxlength="4" />年
    <select name="yue">
    <option selected="selected">1</option>
    <option>2</option>
    <option>3</option>
    <option>4</option>
    <option>5</option>
    <option>6</option>
    <option>7</option>
    <option>8</option>
    <option>9</option>
    <option>10</option>
    <option>11</option>
    <option>12</option>
  </select>月
<input name="ri" type="text" value="1" size="4" maxlength="2" />日</td></tr>
    <tr><td>您通常在网上做些什么：</td><td><input name="shangwang[]" type="checkbox" value="听歌" checked="checked" />听歌
  <input type="checkbox" name="shangwang[]" value="游戏" />游戏
<input type="checkbox" name="shangwang[]" value="聊天" />聊天
<input type="checkbox" name="shangwang[]" value="浏览新闻" />浏览新闻
<input type="checkbox" name="shangwang[]" value="其它" />其它</td></tr>
    <tr><td>文件提交：</td><td><input name="wenjian" type="file" id="wenjian" /></td></tr>
    <tr><td>图像域：</td><td><input name="tuxiang" type="image" src="guan.gif" alt="图像" /></td></tr>
    <tr>
      <td>这里是隐藏信息：</td>
      <td><input name="yincang" type="hidden" id="yincang" value="100" /></td>
    </tr>
    <tr>
    <td>密码：</td><td><input name="mima" type="password" id="mima" value="" size="20" maxlength="20" /></td></tr>
    <tr><td><input type="submit" name="tijiao" value="提交" /><input type="reset" name="chongzhi" value="重置" />
</td>
<td>选择提交方式：<input name="go" type="radio" value="POST" checked="checked" />POST
     <input name="go" type="radio" value="GET" />GET[<font size=2>请将<b>FORM</b>中的<font color=#ff0000><b>method</b></font>值改为<b>GET</b>或<font color=#ff0000><b>POST</b></font></font>]</td></tr></table>
</form>