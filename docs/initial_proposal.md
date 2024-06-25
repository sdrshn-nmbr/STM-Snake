# Initial Project Proposal

Before you start working on this document, you should have spoken to at least two of the course staff (TAs or your lab coordinator) about your project idea. This is to ensure that your project is feasible and that you have a good understanding of the requirements and expectations for the project.

Once you have discussed your project with course staff, complete the initial project proposal by filling out the questions below.  **Only one person from your team needs to do this.**  If you need a guide on writing Markdown (the language this file is written in), [click here](https://guides.github.com/features/mastering-markdown/).

<!--                                               -->
<!--                                               -->
<!-- Do not change/write anything above this line. -->
<!--                                               -->
<!--                                               -->

What is your project name and idea?  Please provide a brief description as if you were advertising it to your first customer.

    Introducing Snake for STM32: a sleek, efficient, and customizable solution for microcontroller enthusiasts. Our Snake software leverages the power of STM32 microcontrollers, offering a seamless gaming experience with smooth graphics and responsive controls. With its modular design, developers can easily tailor gameplay mechanics, levels, and visuals to suit their project needs. Whether you're a hobbyist or a professional, Snake for STM32 provides a fun and engaging platform to showcase the capabilities of your microcontroller. Join us in revolutionizing gaming on embedded systems with Snake for STM32.

What are the main features of your project?  Describe the hardware you plan to use, what parts you wish to acquire (include purchase links for non-kit-based parts), and what your code will do to make the project work.

    The project will be created using the TFT display for output and several buttons for input. The code will read inputs from the user and implement movement functionality.

Provide at least **two** of the external interface formats your project will use.  (See the project lecture for examples.)

   SPI, GPIO

Provide at least **four** of the internal peripheral subsystems your project will use.  (See the project lecture for examples.)

    Timers, DMA, Interrupts, SPI

List some related published projects, products, or articles that you found that are similar to your project.  Provide links to them if possible.  Does your project do anything better or different?  If so, what?

    Someone implemented DOOM on the microcontroller. This is similar to us because it is also a game, but different because it would take much much longer to implement. Our project will be creating Snake which should be simpler but still provide a technical challenge.
   https://www.youtube.com/watch?v=6_3BSTeEb8Q




<!--                                               -->
<!--                                               -->
<!-- Do not change/write anything below this line. -->
<!--                                               -->
<!--                                               -->
<hr>

When you are finished with the proposal, **do not push it to the main branch on GitHub**.  You will submit it by doing the following:
- Before adding and committing this file, create a new branch called `proposal`.  You can do this in the terminal by typing `git checkout -b proposal`.
- Add and commit this file as usual with `git add docs/initial_proposal.md` and `git commit -m "Initial project proposal"`.
- Push the branch to GitHub with `git push origin proposal`.  **Do not just run `git push`, as that will push the main branch.**  This will create a new branch with your proposal on GitHub.  If you ever make updates to the proposal, use this to keep pushing them to the `proposal` branch.
    - Pushing the branch to GitHub will create a new branch on GitHub, and so you may see a message that suggest you create a pull request.  You can just open the URL, or...
- Go to your GitHub project repository.  If you see a banner that says "Compare & pull request", click it.  Navigate to Pull Requests.  You should see a "Compare & pull request" button.  Click it.
- You should see a page with a title like "Comparing proposal with main".  Change the title of the pull request to "Initial project proposal".  You may also write a comment if you wish.  Then click "Create pull request".
- Your PR should now be available.  Course staff will review your proposal (no need to submit the link, we'll find it) and may ask for changes.  Once it is approved, you will receive credit for the proposal.
    - Your submission is considered "approved" if one of the course staff merges the pull request into `main` - **so do not click it yourself** if the option appears!  You are free to start working and push your work to `main`, but you will not be approved for bonus credit unless you have submitted at least one project proposal by March 4, or submitted the revised project proposal by March 25, and have gotten it approved. 
    - If we somehow missed your pull request, send us an email.
