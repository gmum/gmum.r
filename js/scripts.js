$(document).on('ready', function() {
    // Activates scrollspy menu
    $('body').scrollspy({
        target: '#navbar-collapsible',
        offset: 50
    });

    // Smooth scrolling sections
    $('a[href*=#]:not([href=#])').on('click', function() {
        if (location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') && location.hostname == this.hostname) {
            var target = $(this.hash);
            target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
            if (target.length) {
                $('html, body').animate({
                    scrollTop: target.offset().top - 50
                }, 1000);

                return false;
            }
        }
    });

    // Activates highlight.js
    $('pre code').each(function(i, block) {
        hljs.highlightBlock(block);
    });

    // Controls repository history video
    var videoPosition = $('#repository-history').position().top,
        video = $('#repository-history video').get(0);
    video.pause();
    $(window).on('scroll', function() {
        var scrollTop = $(this).scrollTop(),
            topOffset = videoPosition - (0.7 * $(this).height()),
            bottomOffset = videoPosition + 310;
        if (scrollTop >= topOffset && scrollTop <= bottomOffset) {
            video.play();
        } else {
            video.pause();
        }
    });
});
