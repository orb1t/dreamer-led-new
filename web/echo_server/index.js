const express = require('express')
const path = require('path')
const bodyParser = require('body-parser')
const cors = require('cors')
const app = express()
const port = 2832

const store = {};

app.use(express.static(path.join(__dirname, '../data')))
app.use(bodyParser.raw({limit: '50mb'}))
app.use(bodyParser.json())
app.use(cors())

app.get('/', (req, res) => {
    res.sendFile(path.resolve(__dirname, 'index.html'))
})

app.get('/matrix-parameters', (req, res) => {
    res.send({
        width: 16,
        height: 16,
        effects: [
            'slide',
            'bubble',
            'fade'
        ],
        games: [
            'snake',
            'chess',
            'football'
        ],
        refreshingTime: 5000,
        colorsCorrections: [100,100,100],
        speedMax: 1000,
        speed: 20,
        brightness: 100
    })
})

app.post('/pixel', (req, res) => {
    res.sendStatus(200)
})

app.post('/draw', (req, res) => {
    store.img = req.body
    res.sendStatus(200)
})

app.post('/fill-matrix', (req, res) => {
    console.log(req.body);
    res.sendStatus(200)
})

app.get('/matrix-state', (req, res) => {
    res.send(store.img)
})

app.post('/running-text', (req, res) => {
    console.log(req.body);
    res.sendStatus(200)
})

app.post('/clock', (req, res) => {
    console.log(req.body);
    res.sendStatus(200)
})

app.post('/games', (req, res) => {
    console.log(req.body);
    res.sendStatus(200)
})

app.get('/picture-effects', (req, res) => {
    res.send([
        'effect 1',
        'effect 2',
        'effect 3',
        'effect 4',
        'effect 5',
        'effect 6',
        'effect 7',
    ]);
})

app.get('/images', (req, res) => {
    res.send({
        "imageList": [
            "facebook.jpg",
            "twitter.jpg",
            "google-plus.jpg",
            "instagram.jpg",
            "linkedin.jpg",
            "behance.jpg",
            "deviantart.jpg",
            "dribbble.jpg",
            "flickr.jpg",
            "lastfm.jpg",
            "pinterest.jpg",
            "soundcloud.jpg",
            "swarm.jpg",
            "tumblr.jpg"
        ]
    });
})

app.get('/animations', (req, res) => {
    res.send({
        "gifsList": [
            "g1.gif",
            "g2.gif",
            "g3.gif",
            "g4.gif"
        ]
    });
})

app.post('/picture-effects', (req, res) => {
    console.log(req.body);
    res.sendStatus(200);
})

app.post('/send-file', (req, res) => {
    console.log(req.body);
    res.sendStatus(200);
})

app.post('/speed', (req, res) => {
    console.log(req.body);
    res.sendStatus(200);
})

app.post('/brightness', (req, res) => {
    console.log(req.body);
    res.sendStatus(200);
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
})
