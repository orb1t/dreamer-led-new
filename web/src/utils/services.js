import globalStore from '/@store/index';

const BASE_URI = location.host.includes("localhost") ? 'http://localhost:2832' : '';

class Services {

  fillMatrix(body) {
    return fetch(BASE_URI + '/fill-matrix', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify(body),
    })
  }

  sendImgData(body) {
    globalStore.setLoading(1)
    return fetch(BASE_URI + '/draw', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/octet-stream',
      },
      body,
    }).then(() => {
      globalStore.setImageLoaded(body);
    }).finally(() => {
      globalStore.setLoading(0)
    });
  }

  getMatrixState() {
    return fetch(BASE_URI + '/matrix-state').then(response => {
      const reader = response.body.getReader();
      return reader.read();
    }).then(({ value }) => {
//      console.log(value);
      globalStore.setMatrixContent(value);
    });
  }

  getMatrixParameters() {
    return fetch(BASE_URI + '/matrix-parameters')
      .then(res => res.json())
      .then(params => {
        console.log(params)
         globalStore.setMatrixParams(params);
      });
  }

  setSpeed(speed) {
    return fetch(BASE_URI + '/speed', {
      method: 'POST',
      body: JSON.stringify({ speed }),
    });
  }

  setBrightness(brightness) {
    return fetch(BASE_URI + '/brightness', {
      method: 'POST',
      body: JSON.stringify({ brightness }),
    });
  }

  drawSinglePixel(params) {
    return fetch(BASE_URI + '/pixel', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  getImagesFromGallery() {
    globalStore.setLoading(true);
    return fetch('https://dreamer-led.000webhostapp.com/images.php')
        .then(res => res.json())
        .then((response) => {
          if (response.imageList.length) {
            globalStore.setGalleryLinks(response.imageList);
          }
        }).finally(() => {
          globalStore.setLoading();
        });
  }

  getAnimationsFromGallery() {
    globalStore.setLoading(true);
    return fetch('https://dreamer-led.000webhostapp.com/gifs.php')
        .then(res => res.json())
        .then((response) => {
          if (response.gifsList.length) {
            globalStore.setGalleryLinks(response.gifsList);
          }
        }).finally(() => {
          globalStore.setLoading();
        });
  }

  getImagesFromController() {
    globalStore.setLoading(true);
    return fetch(BASE_URI + '/images')
        .then(res => res.json())
        .then((response) => {
          if (response.imageList.length) {
            globalStore.setGalleryLinks(response.imageList);
          }
        }).finally(() => {
          globalStore.setLoading();
        });
  }

  getAnimationsFromController() {
    globalStore.setLoading(true);
    return fetch(BASE_URI + '/animations')
        .then(res => res.json())
        .then((response) => {
          if (response.gifsList.length) {
            globalStore.setGalleryLinks(response.gifsList);
          }
        }).finally(() => {
          globalStore.setLoading();
        });}

  setRunningText(params) {
    return fetch(BASE_URI + '/running-text', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setClock(params) {
    return fetch(BASE_URI + '/clock', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  playGame(params) {
    return fetch(BASE_URI + '/games', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  control(action) {
    if (!action) return;
    return fetch(BASE_URI + '/' + action, { method: 'POST',});
  }

  sendFile(body) {
    // is used for sending any file
    if (!body) return;
    return fetch(BASE_URI + '/send-file', { method: 'POST', body, });
  }

  setPictureEffects(body) {
    return fetch(BASE_URI + '/picture-effects', { method: 'POST', body: JSON.stringify(body),});
  }

  rotate() {
    return fetch(BASE_URI + '/rotate', { method: 'POST' });
  }

  setRefreshingTime(refreshingTime) {
    return fetch(BASE_URI + '/rotate', { method: 'POST', body: JSON.stringify({refreshingTime}) });
  }

  playAllImages() {
    return fetch(BASE_URI + '/play-jpgs', { method: 'GET' });
  }

  playAllAnimations() {
    return fetch(BASE_URI + '/play-gifs', { method: 'GET' });
  }

}

export default new Services();
