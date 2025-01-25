<!-- ---
name: Bug report
about: Create a report to help us improve
title: "[BUG] - Insert Correct title here"
labels: ''
assignees: Skulltrail192

---

**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Desktop (please complete the following information):**
- PC or VM Config: Describe in details the PC or VM settings, including RAM size, Processor Type, Video card name,  type and vendor, hard disk size, storage type (AHCI, IDE, SCSI, NVME, etc) or any relevant info.  
- OS: Is XP 32 bits or 64 bits? Is Server 2003?
- ISO used: tell me the ISO used and link to download.
- Version of OCA
- Method of OCA installation: is clean installation or over another OCA installation? Do you uninstalled the previous OCA version and then installed the new.
- OS updated: do you used Legacy updated to update the OS? Installed before or after OCA?
- BSOD: what is BSOD code? Do you used some custom driver what use ntoskrnl_emu, like storahci, usb3, etc? 

**Additional context**
Add any other context about the problem here. -->


<!-- name: Bug Report
about: Create a report to help us improve
title: "[BUG]: Insert Correct title here"
labels: ["bug"]
assignees:
  - Skulltrail192
body:
  - type: markdown
    attributes:
      value: |
        Thanks for taking the time to fill out this bug report! -->
  <!-- - type: input
    id: contact
    attributes:
      label: Contact Details
      description: How can we get in touch with you if we need more info?
      placeholder: ex. email@example.com
    validations:
      required: false -->
  <!-- - type: textarea
    id: what-happened
    attributes:
      label: What happened?
      description: Also tell us, what did you expect to happen?
      placeholder: Tell us what you see!
      value: "A bug happened!"
    validations:
      required: true
  - type: dropdown
    id: version
    attributes:
      label: Version
      description: What version of our software are you running?
      options:
        - 1.0.2 (Default)
        - 1.0.3 (Edge)
      default: 0
    validations:
      required: true
  - type: dropdown
    id: browsers
    attributes:
      label: What browsers are you seeing the problem on?
      multiple: true
      options:
        - Firefox
        - Chrome
        - Safari
        - Microsoft Edge
  - type: textarea
    id: logs
    attributes:
      label: Relevant log output
      description: Please copy and paste any relevant log output. This will be automatically formatted into code, so no need for backticks.
      render: shell
  - type: checkboxes
    id: terms
    attributes:
      label: Code of Conduct
      description: By submitting this issue, you agree to follow our [Code of Conduct](https://example.com). 
      options:
        - label: I agree to follow this project's Code of Conduct
          required: true -->


name: Bug Report
description: Report a bug encountered while using One-Core-API Binaries
title: "[BUG] - Short description of the issue"
labels: [bug]
body:
  - type: markdown
    attributes:
      value: |
        **Before submitting this issue, please ensure you have:**
        *  Checked if the problem exists in [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary)
        *  Reviewed the existing [Issues](https://github.com/Skulltrail192/One-Core-API-Binaries/issues) to see if your problem has already been reported.
        * **If the problem exists in Canary and is not listed, please fill out the information below.**
        *  **Issues that are non-reproducible will be closed.**

  - type: checkboxes
    id: canary_check
    attributes:
      label: Verification
      description: Verify that you checked these two points before continuing
      options:
          - label: "I have checked if the issue exists in [One-Core-API-Canary](https://github.com/shorthorn-project/One-Core-API-Binaries-Canary)"
          - label: "I have checked the existing [Issues](https://github.com/Skulltrail192/One-Core-API-Binaries/issues)"
  - type: checkboxes
    id: canary_status
    attributes:
      label: Issue Existence in Canary
      description: Does the problem exist in the Canary build?
      options:
        - label: "The problem exists in the Canary build."
    validations:
      required: true
  - type: textarea
    id: problem_description
    attributes:
      label: Concise Problem Description
      description: Briefly describe the issue you are experiencing.
      placeholder: I am experiencing issue X while doing Y, I expect Z but I'm getting A
    validations:
      required: true
  - type: dropdown
    id: system_type
    attributes:
      label: System Type
      description: Is this a physical machine or a virtual machine?
      options:
        - Physical Hardware
        - Virtual Machine
    validations:
      required: true
  - type: textarea
    id: windows_os
    attributes:
      label: Windows OS Edition
      description: Specify the Windows Operating System edition (e.g., Windows XP Professional Service Pack 3).
      placeholder: e.g. Windows XP Professional Service Pack 3
    validations:
       required: true
  - type: dropdown
    id: post_sp_updates
    attributes:
      label: Post-SP Updates Installed?
      description: Were any Post-Service Pack updates installed?
      options:
        - "Yes"
        - "No"
    validations:
      required: true
  - type: dropdown
    id: update_timing
    attributes:
      label: If 'Yes', specify if updates were installed...
      description: Select whether updates were before or after One-Core-API
      options:
        - "Before"
        - "After"
      default: "Before"
    validations:
     required: false
  - type: textarea
    id: installed_software
    attributes:
      label: Installed Software
      description: List any relevant software that might be related to the issue (e.g., Adobe Photoshop CC 2018, Firefox 132, etc.).
      placeholder: e.g. Adobe Photoshop CC 2018, Firefox 132
    validations:
      required: false
  - type: textarea
    id: hardware_specs
    attributes:
      label: Hardware Specifications
      description: "If Physical Hardware: Processor, RAM (type, amount), hard drive type, and capacity (e.g., IDE disk, 120 GB).  If Virtual Machine: Specify the virtual machine configuration details (e.g., Oracle VirtualBox 6.1.0, 2 GB RAM, 120 GB disk, AHCI mode)."
      placeholder: e.g. Processor: Intel Core i7, RAM: 16GB DDR4, Hard Drive: 1TB SSD, OR Virtual Machine: Oracle VirtualBox 6.1.0, 2 GB RAM, 120 GB disk, AHCI mode
    validations:
      required: true
  - type: textarea
    id: steps_to_reproduce
    attributes:
      label: Steps to reproduce the error
      description: "Please provide a detailed step-by-step guide on how to reproduce the error."
      placeholder: "1. Do this... \n2. Then do that... \n3. And I get this issue..."
    validations:
      required: true
  - type: textarea
    id: logs
    attributes:
      label: Attach Relevant Logs (Optional)
      description: Please attach any logs related to the problem.
      placeholder: Attach logs here or provide a link
    validations:
        required: false
  - type: textarea
    id: video_recording
    attributes:
      label: Attach Video Recording (Optional)
      description: Please attach a video recording of the problem if feasible.
      placeholder: Upload video or provide a link
    validations:
        required: false