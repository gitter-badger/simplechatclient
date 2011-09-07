
function appendMessage(html)
{
	var chatElement = document.getElementById('Chat');
	var n = document.createElement('div');
	n.innerHTML = html;
	chatElement.appendChild(n); 
}

function removeFirstMessage()
{
	var chatElement = document.getElementById('Chat');
	chatElement.removeChild(chatElement.firstChild);
}

function clearMessages()
{
	var chatElement = document.getElementById('Chat');
	chatElement.innerHTML = "";
}
