document.addEventListener("DOMContentLoaded", function () {

    var enterGame = false;

    $.fn.enterGameState = function () {
        if (enterGame) {
            $('#game-screen').show('slow');
            $('.runner-canvas').addClass('show-canvas');
            $('#menu').hide('slow');
        }
        else {
            $('#game-screen').hide('slow');
            $('.runner-canvas').removeClass('show-canvas');
            $('#menu').show('slow');
        }
    }

    $.fn.enterGameState();

    $('#playGame').click(function () {
        enterGame = !enterGame;
        $.fn.enterGameState();
    })
    $('.back-menu').click(function () {
        enterGame = !enterGame;
        $.fn.enterGameState();
    })
});

function logout(){
    location.href = './login.html';
}