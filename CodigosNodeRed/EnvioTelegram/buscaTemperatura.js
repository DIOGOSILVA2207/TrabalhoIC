var data = flow.get("temperaturaPlanta");
if (flow.get("temperaturaDesejada") > data){
    msg.payload = {
        chatId: "1331052919",
        type: "message",
        content: "Mensagem automática: A temperatura precisa ser verificada"
    }
    return msg;
}