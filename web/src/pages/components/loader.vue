<template>
    <div class="ui segment global-loader">
        <div class="ui active dimmer inverted">
            <div class="ui massive text loader">Loading</div>
        </div>
    </div>
</template>

<script>
export default {
    name: 'app-loader',
    mounted () {
        console.log('loader')
    }
}
</script>

<style>
.global-loader {
    position: fixed;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: 0;
    background: red;
}
</style>

