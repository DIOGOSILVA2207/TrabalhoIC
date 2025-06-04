var data = flow.get("luminosidadePlanta");
if(flow.get("luminosidadeDesejada") > data){
    msg.payload = {
        chatId: "1331052919",
        type: "message",
        content: "Mensagem automática: A luminosidade precisa ser verificada"
    }
    return msg;
}