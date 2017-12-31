url = 'http://192.168.43.154:5000/'

function getAjax(msg, success) {
    var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Microsoft.XMLHTTP');
    xhr.onreadystatechange = function() {
        if(xhr.readyState == 4 && xhr.status == 200) {
            success(xhr.responseText);
        }
    }

    xhr.open('GET', url + msg, true);
    xhr.send();
    return xhr;
}
