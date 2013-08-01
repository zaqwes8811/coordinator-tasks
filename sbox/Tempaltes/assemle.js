
// копирует все свойства из src в dst,
// включая те, что в цепочке прототипов src до Object
function mixin(dst, src){
    // tobj - вспомогательный объект для фильтрации свойств,
    // которые есть у объекта Object и его прототипа
    var tobj = {}
    for(var x in src){
        // копируем в dst свойства src, кроме тех, которые унаследованы от Object
        if((typeof tobj[x] == "undefined") || (tobj[x] != src[x])){
            dst[x] = src[x];
        }
    }
    // В IE пользовательский метод toString отсутствует в for..in
    if(document.all && !document.isOpera){
        var p = src.toString;
        if(typeof p == "function" && p != dst.toString && p != tobj.toString &&
         p != "\nfunction toString() {\n    [native code]\n}\n"){
            dst.toString = src.toString;
        }
    }
}

// ---- родительский класс ----
function Animal(name, walkSpeed) {
    this.name = name
    this.walkSpeed = walkSpeed
}

// добавляем методы объекта
mixin(Animal.prototype, {
 
    // пример переменной
    distance: 0,
 
    // пример метода
    walk: function(time) {
        this.distance = this.distance + time*this.walkSpeed
    },
 
    toString: function() {
        return this.name+" на расстоянии "+this.distance
    }
})
 
// ---- класс наследник ----
 
function Bird(name, walkSpeed, flySpeed) {
    // вызов родительского конструктора
    Bird.superclass.constructor.call(this, name, walkSpeed)
 
    //this.flySpeed = flySpeed
}
extend(Bird, Animal)
 
mixin(Bird.prototype, {
    fly: function(time) {
        this.distance = this.distance + time*this.flySpeed
    }
})

// usige
bird = new Bird("Птыц", 1, 10)
bird.walk(3)
alert(bird) // => Птыц на расстоянии 3
bird.fly(2)
alert(bird) // => Птыц на расстоянии 23

