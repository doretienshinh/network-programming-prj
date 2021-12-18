document.addEventListener("DOMContentLoaded", function () {
    $('#game-screen').hide();
    $("#mybest-screen").hide();
    $('#menu').show('slow');
    $("#playGame").on("click", function () {
        $('#game-screen').show('slow');
        $('.runner-canvas').addClass('show-canvas');
        $('#menu').hide('slow');
        $("#mybest-screen").hide('slow');
    });
    $(".back-menu").on("click", function () {
        $('#game-screen').hide('slow');
        $('.runner-canvas').removeClass('show-canvas');
        $('#menu').show('slow');
        $("#mybest-screen").hide('slow');
    });
    $("#myBest").on("click", function () {
        $('#myBestScore').html(getCookie('highScore'));
        $("#mybest-screen").show('slow');
        $('#game-screen').hide('slow');
        $('.runner-canvas').removeClass('show-canvas');
        $('#menu').hide('slow');
    });


    //get my Score
    //check login
    if (location.pathname == '/client/game.html') {
        checkLogin();
    }
    
    //validate
    $.fn.validateForm = function () {
        if ($('#username').val() == '' && $('#password').val() == '') {
            $('#loginNoti').html('Please enter username and password!!!');
        }
        else
            if ($('#username').val() == '') {
                $('#loginNoti').html('Please enter username!!!');
            }
            else
                if ($('#password').val() == '') {
                    $('#loginNoti').html('Please enter password!!!');
                }
                else {
                    $('#loginNoti').html('');
                    return true;
                }
    }
    //for login

});

function checkLogin() {
    let user = getCookie("username");
    if (user == null) {
        location.href = './login.html'
    }
}
function getCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') c = c.substring(1,c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
    }
    return null;
}

function set_cookie(name, value) {
    document.cookie = name + '=' + value + '; Path=/;';
}
function delete_cookie(name) {
    document.cookie = name + '=;expires=Thu, 01 Jan 1970 00:00:01 GMT;';
};
function logout() {
    delete_cookie('username');
    delete_cookie('highScore');
    location.href = './login.html';
}

