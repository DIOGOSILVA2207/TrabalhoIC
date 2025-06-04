var data = flow.get("humidadeSolo");
if (flow.get("humidadeSoloDesejada") > data){
    msg.payload = {
        chatId: "1331052919",
        type: "message",
        content: "Mensagem automática: A humidade do Solo precisa ser verificada"
    }
    return msg;
}