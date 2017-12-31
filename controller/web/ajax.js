function getAjax(msg, success) {
    var xhr = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Microsoft.XMLHTTP');
    xhr.onreadystatechange = function() {
        if(xhr.readyState == 4 && xhr.status == 200) {
            success(xhr.responseText);
        }
    }
    url = getcurURL();
    xhr.open('GET', url + msg, true);
    xhr.send();
    return xhr;
}

function getcurURL() {
    var wholeURL = window.location.href;
    var temp = wholeURL.indexOf("5000/")+4;
    var result = wholeURL.substr(0,temp) + '/';
    return result;
}
