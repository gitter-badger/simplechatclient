
function appendMessage(html)
{
	var div = document.getElementById('Chat').getElementsByTagName('div');
	if (div.length > 350)
	{
		var chatElement = document.getElementById('Chat');
		chatElement.removeChild(chatElement.firstChild);
	}

	var chatElement = document.getElementById('Chat');
	var n = document.createElement('div');
	n.innerHTML = html;
	chatElement.appendChild(n); 
}

function clearMessages()
{
	var chatElement = document.getElementById('Chat');
	chatElement.innerHTML = "";
}
