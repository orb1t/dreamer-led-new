<template>
    <div v-if="inited">
        <div class="ui container main">

            <app-toolbar
                v-show="currentPage && !currentPage.noToolbar"
            ></app-toolbar>

            <component :is="currentPage && currentPage.component" />

            <div
                class="main__menu"
                :class="isMenuOpened ? 'main__menu--opened' : ''"
                @click="isMenuOpened = !isMenuOpened"
            >
                <button
                    v-for="(tab, key) in tabs"
                    class="ui circular icon button massive main__menu-item"
                    :class="[
                        key === currentTab ? 'main__menu-item--active' : '',
                        // tab.color
                    ].join(' ')"
                    :key="key"
                    @click="currentTab = key"
                >
                    <i
                        class="icon"
                        :class="tab.icon"
                    ></i>
                    <span>{{ tab.label }}</span>
                </button>
            </div>
        </div>
    </div>
</template>

<script>
import store from '/@store/index';
import services from '/@utils/services';

import AppToolbar from '/@components/toolbar.vue';
import AppPicture from '/@pages/picture.vue';
import AppText from '/@pages/text.vue';
import AppEffects from '/@pages/effects.vue';
import AppGames from '/@pages/games.vue';
import AppSettings from '/@pages/settings.vue';

export default {
    name: 'App',
    components: {
        AppToolbar
    },
    provide: {
        store,
    },
    data: () => {
        return {
            inited: false,
            tabs: {
                'Picture': {
                    label: 'Picture',
                    icon: 'image',
                    component: AppPicture,
                    color: 'red'
                },
                'Text/Clock': {
                    label: 'Text/Clock',
                    icon: 'keyboard outline',
                    component: AppText,
                    color: 'green'
                },
                'Effects for picture': {
                    label: 'Effects for picture',
                    icon: 'film',
                    component: AppEffects,
                    color: 'blue'
                },
                'Games': {
                    label: 'Games',
                    icon: 'chess',
                    component: AppGames,
                    color: 'yellow'
                },
                'Settings': {
                    label: 'Settings',
                    icon: 'cogs',
                    component: AppSettings,
                    color: 'orange',
                    noToolbar: true
                },
            },
            isMenuOpened: false,
            openedPage: ''
        };
    },
    computed: {
        currentPage () {
            return this.tabs[this.currentTab];
        },
        currentTab: {
            get () {
                return this.openedPage || localStorage.currentTab || Object.keys(this.tabs)[0];
            },
            set (tab) {
                if (this.isMenuOpened) {
                    this.openedPage = tab;
                    localStorage.currentTab = tab;
                }
            }
        },
    },
    methods: {
        initSockets() {
            try {
                const webSocket = new WebSocket(`ws://${location.hostname}/echo`);

                webSocket.onopen = event => {
                    console.log('onopen');
                    // webSocket.send("Hello Web Socket!");
                };

                webSocket.onmessage = event => {
                    console.log('onmessage, ' + event.data);
                    // event.data.
                    if ( event.data instanceof Blob ) {
                      let fr = new FileReader();
                      fr.readAsArrayBuffer(event.data);
                      fr.addEventListener("loadend", ev => {
                        let arr = new Uint8Array(ev.target.result);
                        console.log(arr);
                        store.setMatrixContent(arr);
                      });
                    }


                    // const reader = event.data.getReader();
                    // reader.read().then(({ value }) => {
                    //     store.setMatrixContent(value);
                    // });
                };

                webSocket.onclose = event => {
                    console.log('onclose');
                };
            } catch(e) {
                console.log(e);
            }
        }
    },
    mounted() {
        services
            .getMatrixParameters()
            .then(() => {
                this.inited = true;
            });
        this.initSockets();
    }
}
</script>

<style>
* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    font-family: sans-serif;
}

.main {
    position: relative;
    min-height: 100vh;
    padding: 20px 0 100px;
}

.main > .segment {
    min-height: 95vh;
}

.main__menu {
    position: fixed;
    bottom: 20px;
    right: 20px;
    background: rgba(0, 0, 0, 0);
    transition: all ease 0.5s;
}

.main__menu--opened {
    width: 100vw;
    height: 100vh;
    top: 0px;
    left: 0px;
    z-index: 9;
    background: rgba(0, 0, 0, 0.8);
}

.main__menu--opened .main__menu-item:nth-child(2) {bottom: 90px;}
.main__menu--opened .main__menu-item:nth-child(3) {bottom: 160px;}
.main__menu--opened .main__menu-item:nth-child(4) {bottom: 230px;}
.main__menu--opened .main__menu-item:nth-child(5) {bottom: 300px;}
.main__menu--opened .main__menu-item:nth-child(6) {bottom: 370px;}
.main__menu--opened .main__menu-item:nth-child(7) {bottom: 440px;}
.main__menu--opened .main__menu-item:nth-child(8) {bottom: 610px;}
.main__menu--opened .main__menu-item:nth-child(9) {bottom: 680px;}

.main__menu-item {
    position: fixed;
    bottom: 20px;
    right: 20px;
    transition: all ease .5s;
    box-shadow: rgba(0, 0, 0, 0.5) -5px -4px 13px -3px inset !important;
}

.main__menu-item--active {
    z-index: 999999;
}

.main__menu-item span {
    position: fixed;
    opacity: 0;
    right: 0px;
    transition: all 0.5s ease 0s;
    color: #fff;
}

.main__menu--opened .main__menu-item span {
    right: 100px;
    opacity: 1;
}

.global-loader {
    position: fixed;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: 0;
}

.picture__row {
    display: flex;
}

.picture__column {
    width: 2px;
    height: 2px;
}

.drawer {
    max-width: 600px;
    margin: 10px auto;
}

.drawer__row {
    display: flex;
}

.drawer__col {
    border: 1px solid #f5f5f5;
}

.picker_wrapper {
    left: 0 !important;
    right: 0 !important;
    margin: auto !important;
}

.picker_arrow {
    display: none !important;
}

.upload__button {
    text-align: center;
}

.upload__drawer {
    /* display: none; */
    margin: 10px auto;
}

.ui.header:first-child {
    margin-top: 1.14285714em;
}

.mb-10 {
    margin-bottom: 10px !important;
}

.center {
    text-align: center;
}
</style>
