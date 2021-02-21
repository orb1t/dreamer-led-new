<template>
    <div class="gallery">
        <h4 class="ui horizontal divider header">
            <i class="folder open icon"></i>
            Gallery
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="gallery-handlers mb-10">
                    <select
                        class="ui dropdown fluid mb-10"
                        v-model="mode"
                        @change="loadGallery"
                    >
                        <option :value="modes.imagesFromGallery">Images from gallery</option>
                        <option :value="modes.imagesFromMemory">Images from memory</option>
                        <option :value="modes.animationsFromGallery">Animations from gallery</option>
                        <option :value="modes.animationsFromMemory">Animations from memory</option>
                    </select>
                    <button
                        v-if="!isFromMemory && mode && store.state.gallery.links.length"
                        class="ui compact icon button"
                        :class="multiMode ? 'green' : 'grey'"
                        @click="multiMode = !multiMode"
                    >
                        <i class="icon" :class="multiMode ? 'tasks' : 'list ul'"></i>
                    </button>
                    <button
                        v-if="mode && store.state.gallery.links.length"
                        class="ui green compact icon button"
                        @click="playAll"
                    >
                        <i class="icon" :class="isFromMemory ? 'play' : 'save'"></i>
                    </button>
                </div>
                <div
                    v-if="store.state.gallery.links.length"
                    class="ui small images mb-10"
                >
                    <span
                        v-for="(gif, i) in store.state.gallery.links"
                        :key="gif"
                        class="images__item"
                        :class="{
                            'images__item--active': selectedImages.includes(prefix + gif)
                        }"
                    >
                        <img
                            v-if="blobList[gif]"
                            :ref="'gif' + i"
                            :src="blobList[gif]"
                            @click="selectGif(prefix + gif)"
                        />
                    </span>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import store from '/@store/index';

export default {
    name: 'app-animation-gallery',
    inject: ['store'],
    data () {
        return {
            mode: null,
            modes: {
                imagesFromGallery: '0',
                imagesFromMemory: '1',
                animationsFromGallery: '2',
                animationsFromMemory: '3',
            },
            multiMode: false,
            selectedImages: [],
            blobList: {}
        }
    },
    computed: {
        prefix () {
            switch (this.mode) {
                case this.modes.imagesFromGallery:
                    return 'https://dreamer-led.000webhostapp.com/image.php?image=';
                case this.modes.animationsFromGallery:
                    return 'https://dreamer-led.000webhostapp.com/gif.php?gif=';
                default:
                    return '';
            }
        },
        isFromMemory() {
            switch (this.mode) {
                case this.modes.imagesFromGallery:
                case this.modes.animationsFromGallery:
                    return false;
                case this.modes.imagesFromMemory:
                case this.modes.animationsFromMemory:
                    return true;
            }
        },
        readyLinks() {
            if (this.multiMode) {
                return this.selectedImages
            }
            return this.store.state.gallery.links;
        }

    },
    watch: {
        'store.state.gallery.links'() {
            const links = [...this.store.state.gallery.links];
            const loadImageBlob = (links) => {
                const src = links.pop();

                if (!src) return;

                fetch(this.prefix + src)
                .then(res => res.blob())
                .then(res => {
                    const reader = new FileReader();
                    reader.readAsDataURL(res); 
                    reader.onloadend = () => {
                        this.blobList[src] = reader.result;    
                    }
                })
                // loadImageBlob(links);
                setTimeout(() => {  console.log("loadImageBlob inner with timeout 2000ms"); loadImageBlob(links); }, 500);
            }
            setTimeout(() => {  console.log("loadImageBlob with timeout 2000ms"); loadImageBlob(links); }, 500);
        }
    },
    methods: {
        async loadGallery () {
            this.store.clearGalleryLinks();
            switch (this.mode) {
                case this.modes.imagesFromGallery:
                    return await services.getImagesFromGallery();
                case this.modes.imagesFromMemory:
                    return await services.getImagesFromController();
                case this.modes.animationsFromGallery:
                    return await services.getAnimationsFromGallery();
                case this.modes.animationsFromMemory:
                    return await services.getAnimationsFromController();
            }
        },
        selectGif (url) {
            if (this.multiMode) {
                this.selectMultiImages(url);
            } else {
                this.selectSingleImage(url);
            }
        },
        selectMultiImages(url) {
            const index = this.selectedImages.indexOf(url);
            if (index >= 0) {
                this.selectedImages.splice(index, 1);
            } else {
                this.selectedImages.push(url);
            }
        },
        selectSingleImage (url) {
            let [fileName] = url.match(/[\d\w]+\.(jpg|gif)$/) || [];

            return fetch(url)
                .then(e => e.blob())
                .then(blob => {
                    const file = new File([blob], fileName);
                    const formData = new FormData();
                    formData.append('update', file);

                    return services.sendFile(formData);
                })
        },
        async playAll () {
            switch (this.mode) {
                case this.modes.imagesFromGallery:
                    return this.sendAllRecursively();
                case this.modes.imagesFromMemory:
                    return await services.playAllImages();
                case this.modes.animationsFromGallery:
                    return this.sendAllRecursively();
                case this.modes.animationsFromMemory:
                    return await services.playAllAnimations();
            }
        },
        sendAllRecursively () {
            const send = async (i) => {
                const fileName = this.readyLinks[i];
                if (!fileName) {
                    return;
                }
                await this.selectGif(this.prefix + fileName);
                await send(i + 1);
            }

            send(0);
        }
    },
    mounted () {
        $('.ui.dropdown').dropdown();
    },
    unmounted () {
        this.store.clearGalleryLinks();
    },
}
</script>

<style>
.images {
    display: flex;
    justify-content: space-around;
    flex-wrap: wrap;
}

.images > span {
    margin-bottom: 10px;
}

.gallery .ui.images img {
    width: 50px;
    height: 50px;
}

.images__item {
    overflow: hidden;
    border-radius: 4px;
}

.images__item--active {
    position: relative;
    top: -2px;
    left: -2px;
    box-shadow: grey 2px 2px 3px 2px;
}

.gallery-handlers {
    display: flex;
    align-items: center;
}

.gallery-handlers > div {
    margin: 0 !important;
}

.gallery-handlers > button {
    margin-left: 10px !important;
}

</style>
