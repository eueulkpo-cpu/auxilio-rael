let utimotempo =0;

function loop(tempo) {
    const delta = tempo - utimotempo;
    utimoTempo = tempo;

    //Atualiza so o necesasio

    requestAnimationFrame(loop);
};

requestAnimationFrame(loop);

const botao = document.getElementById("Otimiza");
const botao = document.getElementById("status");

botao.addEventListener("click"), function () {
    status.texcontent = "on";
}