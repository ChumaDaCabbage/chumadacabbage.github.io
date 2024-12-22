function scrollToFooter() 
{
  const footerElement = document.querySelector('.footer');
  footerElement.scrollIntoView({ behavior: 'smooth' });

  //Highlight the text elements in the footer
  const textElements = document.querySelectorAll('.footer .contactInfo .item');
  
  textElements.forEach((textElement) => 
  {
    textElement.classList.add('highlight');
    
    //Remove the highlight class after a delay (1 second)
    setTimeout(() => {textElement.classList.remove('highlight');}, 1000);
  });
}