	<script type="text/javascript">
		$(document).ready(function(){
			$(".btn-slide").click(function(){
				$("#panel").slideToggle("high");
				$(this).toggleClass("active"); return false;
			});
		});
	</script>