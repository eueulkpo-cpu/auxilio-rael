let velocidade = 10;

document.addEventListener("touchmove" , () => {
    velocidade *= 0.85; // reduz a velocidade em 5% a cada movimento
    console.log("velocidade:", velocidade);
});

    const botao = document.getElementById("AimBot");
    const status = document.getElementById("status");

    botao.addEventListener("click", function () {
        status.textContent = "ON";
    
});
//loop gira o perssonagem
 function gameLoop() {
  //Ler a entrada do jogador
    if (input.touches) {
        console.log('tocando em: (${input.x}, ${input.y})');
        console.log('movimento: (${input.deltaX} , ${input.deltaY}) ');

        //exemplo perssonagem
     let velocidade = 10;

document.addEventListener("touchmove" , () => {
    velocidade *= 0.85; // reduz a velocidade em 5% a cada movimento
    console.log("velocidade:", velocidade);
});

    }
}    

 
 const botao2 = document.getElementById("HEADTRICK");
 const status2 = document.getElementById("status2");

botao2.addEventListener("click", function () {
    status2.textContent = "ON";
});