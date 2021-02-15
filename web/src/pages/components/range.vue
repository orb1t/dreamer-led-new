<template>
    <div class="wrapper">
        <span>{{ min }}</span>
        <input
            type="range"
            :value="modelValue"
            :min="min"
            :max="max"
            @change="change"
        />
        <span>{{ max }}</span>
    </div>
</template>

<script>
export default {
    name: 'app-range',
    props: {
        modelValue: {
            type: [String, Number],
            required: true,
            default: 3
        },
        min: {
            type: Number,
            default: 0
        },
        max: {
            type: Number,
            default: 100
        },
    },
    methods: {
        change(event) {
            this.$emit('update:modelValue', event.target.value)
        }
    }
}
</script>

<style scoped>
.wrapper {
    display: flex;
    align-items: center;
    justify-content: space-between;
}
.wrapper input {
    width: 85%;
}
</style>
