function menu_goto( menuform )
{
  // Generated by thesitewizard Navigation Menu Wizard 2.3.6
  // Visit http://www.thesitewizard.com/ to get your own
  // customized navigation menu FREE!
  var baseurl = 'http://ec2-54-179-162-196.ap-southeast-1.compute.amazonaws.com/' ;
  selecteditem = menuform.url.selectedIndex ;
  newurl = menuform.url.options[ selecteditem ].value ;
  if (newurl.length != 0) {
    location.href = baseurl + newurl ;
  }
}
