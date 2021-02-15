<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="film icon"></i>
            Effects
        </h4>
        <div class="ui card center" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <select ref="dropdown" v-model="selectedEffect" class="ui search dropdown">
                        <option value="">Select effect</option>
                        <option
                            v-for="(effect, i) in effectsList"
                            :key="effect"
                            :value="i"
                        >{{ effect }}</option>
                    </select>
                </div>
            </div>
        </div>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="center">
                    <button
                        class="ui right labeled icon button green"
                        @click="apply"
                    >
                        <i class="check icon"></i>
                        Apply
                    </button>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from "/@utils/services";

export default {
    name: 'app-effects',
    inject: ['store'],
    data() {
        return {
            selectedEffect: 0,
        };
    },
    computed: {
        effectsList() {
            return this.store.state.matrixParams.effects;
        }
    },
    methods: {
        apply() {
            services.setPictureEffects({
                effect: this.selectedEffect,
            })
        },
    },
}
</script>
