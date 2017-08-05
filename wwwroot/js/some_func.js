function is_email(str){
	var reg = /^([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+(.[a-zA-Z0-9_-])+/; 
	return reg.test(str); 
}
function request_email(){
	email = prompt("请输入你的email,检测完成后会将结果会发送至你邮箱!");
	if(!is_email(email)){
		alert("邮箱输入有误!");
		history.go(-1);
	}
	document.getElementById("email").value = email;
	document.getElementById("scan").submit();
}