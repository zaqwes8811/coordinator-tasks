
function load() 
{
  
}

function get_max_power_value() 
{
  var kMax = 9000;
  return kMax;
}

function get_current_power() 
{
  var g_current_power = 100;
  return g_current_power;
}

function set_power_view(power_value) 
{
  // 
  var ind = "Мощность "+power_value+" Вт";
  
  // Установить как-то нужно
}

// type: 'down_power' or 'up_power'
function send_tune_power_request(type) 
{
  var script_name = 'action.cgi'
  var error_handler = function(e, message) {
    alert("При установке можности возникла ошибка: "+e);
  };
  
  $.ajax({
    url: script_name,
    cache: false,
    type: 'GET',
    timeout: 10000,  // вроде бы не везде помогает
    data: {action: type},
  }).done(function(response) {
    // Проверяем успешно ли
    
    alert(response);
  }).error(function(e) {
    error_handler(e);
  });
}

/// Actions

function down_power() 
{
  if (get_current_power() > 0)  {
    // можно понижать
    send_tune_power_request('down_power');
  }
}

function up_power() 
{
  if (get_current_power() <= get_max_power_value()) {
    // можно повышать
    send_tune_power_request('up_power');
  }
}