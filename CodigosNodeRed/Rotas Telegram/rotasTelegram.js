var opts = {
    reply_markup: JSON.stringify({
        "inline_keyboard": [[
            {
                "text":"Ver humidade do Ar",
                "callback_data": "/humidadeAr"
            },
            {
                "text":"Ver humidade do Solo",
                "callback_data": "/humidadeSolo"
            },
            {
                "text": "Ver temperatura",
                "callback_data": "/temperatura"
            },
            {
                "text": "Ver Luminosidade",
                "callback_data": "/luminosidade"
            },
            {
                "text": "Adicionar Rega",
                "callback_data": "/addRega"
            },
            {
                "text": "Ver Histórico de Rega",
                "callback_data": "/historicoRega"
            },
            {
                "text": "Ver Status da Planta",
                "callback_data": "/status"
            },
            ]
        ]
    })
};

msg.payload = {
    chatId: "1331052919",
    type: "message",
    content: "O que deseja saber?",
    options: opts
}
return msg;