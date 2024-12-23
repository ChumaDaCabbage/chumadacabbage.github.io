//Function to scroll to contact section
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

 //Function to load the code from a file
 async function loadCode() 
 {
  try 
  {
    const response = await fetch('overlap.cpp');  //The file to fetch
    if (response.ok) 
    {
      const code = await response.text();
      document.getElementById('overlappSnippet').innerHTML = `<pre class="prettyprint lang-cpp">${code}</pre>`;
      PR.prettyPrint();  //Run Google Prettify to style the code
    } 
    else 
    {
      console.error('Failed to load the code file');
    }
  } 
  catch (error) 
  {
    console.error('Error loading the code:', error);
  }
}