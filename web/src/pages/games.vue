<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="chess icon"></i>
            Games
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">

                    <select
                        v-model="currentGame"
                        class="ui dropdown"
                        id="dropdown"
                    >
                        <option
                            v-for="game in gamesList"
                            :key="game"
                            :value="game"
                        >
                            {{ game }}
                        </option>
                    </select>
                    
                    <button
                        class="ui left labeled icon button green"
                        :disabled="!currentGame"
                        @click="play"
                    >
                        <i class="play icon"></i>
                        Play
                    </button >

                </div>
            </div>
        </div>
        <h4 class="ui horizontal divider header">
            <i class="arrows alternate icon"></i>
            Controls
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10 game__controls">
                    
                    <button
                        class="ui icon button"
                        :disabled="!gameStarted"
                        @click="up"
                    >
                        <i class="caret square up outline icon"></i>
                    </button>
                    
                    <div>
                        <button
                            class="ui icon button left"
                            :disabled="!gameStarted"
                            @click="left"
                        >
                            <i class="caret square left outline icon"></i>
                        </button>
                        
                        <button
                            class="ui icon button right"
                            :disabled="!gameStarted"
                            @click="right"
                        >
                            <i class="caret square right outline icon"></i>
                        </button>
                    </div>
                    
                    <button
                        class="ui icon button"
                        :disabled="!gameStarted"
                        @click="down"
                    >
                        <i class="caret square down outline icon"></i>
                    </button>

                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';

export default {
    name: 'app-games',
    inject: ['store'],
    data() {
        return {
            currentGame: '',
            gameStarted: false
        };
    },
    computed: {
        gamesList() {
            return this.store.state.matrixParams.games || [];
        },
    },
    methods: {
        play() {
            if (!this.currentGame) return;

            services.playGame({
                game: this.currentGame,
            }).then(() => this.gameStarted = true);
        },
        up() {
            services.control('up')
        },
        right() {
            services.control('right')
        },
        down() {
            services.control('down')
        },
        left() {
            services.control('left')
        },
        watchKeys(e) {
            const keys = {
                37: 'left',
                38: 'up',
                39: 'right',
                40: 'down',
            }
            services.control(keys[e.keyCode]);
        }
    },
    mounted() {
        $('#dropdown').dropdown();

        document.addEventListener('keydown', this.watchKeys);
    },
    unmounted() {
        document.removeEventListener('keydown', this.watchKeys);
    }
}
</script>

<style>


.game__controls {
    text-align: center;
}

.game__controls > div {
    display: flex;
    justify-content: center;
}

.game__controls button.left {
    margin-right: 45px !important;
}

.game__controls button.right {
    margin-left: 45px !important;
}

.game__controls button i {
    font-size: 90px;
    height: 90px;
    width: 90px;
    display: flex;
    align-items: center;
    justify-content: center;
}
</style>
