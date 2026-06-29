let utimoTempo =0;

function loop(tempo) {
    const delta = tempo - utimoTempo;
    utimoTempo = tempo;

    //Atualiza so o necesasio

    requestAnimationFrame(loop);
};

requestAnimationFrame(loop);


const botao = document.getElementById("otimiza");
const status = document.getElementById("status");


botao.addEventListener("click", function () {
    status.texcontent = "ON";
    alert("ativado");
    console.log("ativado");
    
});
