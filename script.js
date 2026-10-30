const switchBtn = document.getElementById('mira');

switchBtn.addEventListener('change', function () {
    if (this.checked) {
        minhaFuncao();
    } else {
        desativarFuncao();
    }
});

function minhaFuncao() {
    console.log('Função ativada');
}

function desativarFuncao() {
    console.log('Função desativada');
}
