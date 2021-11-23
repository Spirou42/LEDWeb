
$( document ).ready(function() {
    $( "a" ).addClass( "test" );
    $( "a" ).click(function( event ) {
        alert( "As you can see, the link no longer took you to %HOSTNAME%" );
        event.preventDefault();
 
    });
 
});

$( function() {
  $("#SystemPalettes").selectmenu();
  $("#SystemEffects").selectmenu();
  $(".ui-controlgroup").controlgroup(); 
  } );