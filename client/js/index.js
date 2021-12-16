document.addEventListener("DOMContentLoaded", function () {
    enterGameState();
});
var enterGame = false;
function enterGameState() {
    if (enterGame) {
        $('#game-screen').show('slow');
        $('#menu').hide();
    }
    else {
        $('#game-screen').hide();
        $('#menu').show('slow');
    }
}

function playGame() {
    enterGame = !enterGame;
    enterGameState();
}