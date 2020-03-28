(defrule check-the-hydrological-survey(not(check-hydrological-survey ?)) => (printout t "What does the hydrological survey indicate? "crlf" 1.Land Erosion "crlf" 2.Risk of flooding "crlf" 3.Minimal effect "crlf" ") (assert(check-hydrological-survey(read))))
(defrule land-erosion(check-hydrological-survey 1) => (assert(erosion)) (printout t "Reject - completely unsuitable due to likelihood of land erosion." crlf) (retract 1))
(defrule is-there-run-off-or-flooding(check-hydrological-survey 2) => (assert(significant-run-off-or-flooding)) (printout t "Reject - completely unsuitable due to risk of flooding." crlf) (retract 1))
(defrule is-there-minimal-effect-or-no-effect(check-hydrological-survey 3) (not(check-energy-production ?)) => (printout t "Is the energy production: (1.low "crlf" 2.medium "crlf" 3. high)?") (assert(check-energy-production(read))))
(defrule low(check-energy-production 1) => (assert(low-energy)) (printout t "Reject – completely unsuitable due to not being profitable." crlf))
 
(defrule while-high-energy-calculate-wildlife-score
(check-energy-production 3) 
=> 
(printout t "What is the number of endangered bird species ")
(bind ?n1 (read-number))
(printout t "What is the number of scarce bird species ")
(bind ?n2 (read-number))
(printout t "What is the number of common bird species ")
(bind ?n3 (read-number))
(bind ?wildlife-score-total (+ (* ?n3 1) (* ?n2 2) (* ?n1 3)))
(printout t "The wildlife score is " ?wildlife-score-total crlf)
(assert (wildlife-score ?wildlife-score-total))
)

(defrule while-high-energy-and-at-least-20 
(wildlife-score ?is-this-score-at-least-20)
(test (>= ?is-this-score-at-least-20 20))
=>
(printout t "Reject - completely unsuitable due to wildlife impact" crlf)
)

(defrule while-high-energy-and-more-than-10-but-less-than-20 
(wildlife-score ?is-this-score-more-than-10-but-less-than-20)
(test(>= ?is-this-score-more-than-10-but-less-than-20 10))
(test (< ?is-this-score-more-than-10-but-less-than-20 20))
(not(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact ?))
=>
(printout t "Does the hydrological survey indicate: ("crlf" 1.Very distant "crlf" 2.Other cases)?") 
(assert(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact(read))) 
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-and-very-distant
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact 1) 
(not(high-energyproduction_more-than-10-but-less-than-20_check-the-visual-impact_very-distant ?))
=>
(printout t "Is the sound impact: (1.Quiet or 2.Loud)?")
(assert(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant(read)))
)

(defrule other
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact 2)
(not(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other(read)))
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-check-the-geological-survey-1
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-check-the-geological-survey-2
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-and-loud
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant 2)
=>
(printout t "Reject - completely unsuitable due to impact on population")
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-and-quiet
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant 1)
(not(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey(read)))
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-and-check-the-geological-survey-1
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-high-energy-and-more-than-10-but-less-than-20-and-check-the-geological-survey-2
(high-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)


(defrule while-high-energy-and-no-more-than-10
(wildlife-score ?is-this-score-no-more-than-10)
(test(< ?is-this-score-no-more-than-10 10))
(not(high-energy-production_no-more-than-10_check-the-visual-impact ?))
=>
(printout t "Does the hydrological survey indicate: ("crlf" 1.Close "crlf" 2.Fairly distant 3.Very distant)?") 
(assert(high-energy-production_no-more-than-10_check-the-visual-impact(read)))
)

(defrule while-high-energy-and-no-more-than-10-and-close
(high-energy-production_no-more-than-10_check-the-visual-impact 1)
(not(high-energy-production_no-more-than-10_check-the-visual-impact_close ?))
=>
(printout t "Is the sound impact: (1.Quiet or 2.Loud)?")
(assert(high-energy-production_no-more-than-10_check-the-visual-impact_close(read)))
)

(defrule while-high-energy-and-no-more-than-10-and-quiet
(high-energy-production_no-more-than-10_check-the-visual-impact_close 1)
(not(high-energy-production_no-more-than-10_check-the-visual-impact_close_quiet ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(high-energy-production_no-more-than-10_check-the-visual-impact_close_quiet(read)))
)

(defrule while-high-energy-and-no-more-than-10-and-quiet-check-the-geological-survey
(high-energy-production_no-more-than-10_check-the-visual-impact_close_quiet 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-high-energy-and-no-more-than-10-check-the-geological-survey
(high-energy-production_no-more-than-10_check-the-visual-impact_close_quiet 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-high-energy-and-no-more-than-10-loud
(high-energy-production_no-more-than-10_check-the-visual-impact_close 2)
=>
(printout t "Reject - completely unsuitable due to impact on population")
)

(defrule while-high-energy-and-no-more-than-10-fairly-distant
(high-energy-production_no-more-than-10_check-the-visual-impact 2)
(not(high-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant ?))
=>
(printout t "Is the sound impact: (1.Stable or partly stable or 2.Unstable)?")
(assert(high-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant(read)))
)

(defrule while-high-energy-and-fairly-distant-and-unstable
(high-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-high-energy-and-fairly-distant-is-it-stable-or-partly-stable 
(high-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant 1)
=>
(printout t "Accept as second best quality location - stabilize the land if necessary")
)

(defrule while-high-energy-and-very-distant
(high-energy-production_no-more-than-10_check-the-visual-impact 3)
(not(high-energy-production_no-more-than-10_check-the-visual-impact_very-distant ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable 2.partly stable or 3.Unstable)?")
(assert(high-energy-production_no-more-than-10_check-the-visual-impact_very-distant(read)))
)

(defrule while-high-energy-and-no-more-than-10-unstable
(high-energy-production_no-more-than-10_check-the-visual-impact_very-distant 3)
=>
(printout t "Reject - completely unsuitable due to unstable due to unstable ground")
)

(defrule while-high-energy-and-no-more-than-10-partly-stable
(high-energy-production_no-more-than-10_check-the-visual-impact_very-distant 2)
=>
(printout t "Accept as second best quality location - stabilize the land if necessary")
)

(defrule while-high-energy-and-no-more-than-10-stable
(high-energy-production_no-more-than-10_check-the-visual-impact_very-distant 1)
=>
(printout t "Accept - an ideal location. Start building it now")
)


(defrule while-medium-energy-calculate-wildlife-score
(check-energy-production 3) 
=> 
(printout t "What is the number of endangered bird species ")
(bind ?n1 (read-number))
(printout t "What is the number of scarce bird species ")
(bind ?n2 (read-number))
(printout t "What is the number of common bird species ")
(bind ?n3 (read-number))
(bind ?wildlife-score-total (+ (* ?n3 1) (* ?n2 2) (* ?n1 3)))
(printout t "The wildlife score is " ?wildlife-score-total crlf)
(assert (wildlife-score ?wildlife-score-total))
)

(defrule while-medium-energy-and-at-least-20 
(wildlife-score ?is-this-score-at-least-20)
(test (>= ?is-this-score-at-least-20 20))
=>
(printout t "Reject - completely unsuitable due to wildlife impact" crlf)
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20 
(wildlife-score ?is-this-score-more-than-10-but-less-than-20)
(test(>= ?is-this-score-more-than-10-but-less-than-20 10))
(test (< ?is-this-score-more-than-10-but-less-than-20 20))
(not(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact ?))
=>
(printout t "Does the hydrological survey indicate: ("crlf" 1.Very distant "crlf" 2.Other cases)?") 
(assert(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact(read))) 
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-and-very-distant
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact 1) 
(not(medium-energyproduction_more-than-10-but-less-than-20_check-the-visual-impact_very-distant ?))
=>
(printout t "Is the sound impact: (1.Quiet or 2.Loud)?")
(assert(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant(read)))
)

(defrule while-medium-and-other
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact 2)
(not(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other(read)))
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-check-the-geological-survey-1
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-check-the-geological-survey-2
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_other 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-and-loud
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant 2)
=>
(printout t "Reject - completely unsuitable due to impact on population")
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-and-quiet
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant 1)
(not(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey(read)))
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-and-check-the-geological-survey-1
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-medium-energy-and-more-than-10-but-less-than-20-and-check-the-geological-survey-2
(medium-energy-production_more-than-10-but-less-than-20_check-the-visual-impact_very-distant_check-geological-survey 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)


(defrule while-medium-energy-and-no-more-than-10
(wildlife-score ?is-this-score-no-more-than-10)
(test(< ?is-this-score-no-more-than-10 10))
(not(medium-energy-production_no-more-than-10_check-the-visual-impact ?))
=>
(printout t "Does the hydrological survey indicate: ("crlf" 1.Close "crlf" 2.Fairly distant 3.Very distant)?") 
(assert(medium-energy-production_no-more-than-10_check-the-visual-impact(read)))
)

(defrule while-medium-energy-and-no-more-than-10-and-close
(medium-energy-production_no-more-than-10_check-the-visual-impact 1)
(not(medium-energy-production_no-more-than-10_check-the-visual-impact_close ?))
=>
(printout t "Is the sound impact: (1.Quiet or 2.Loud)?")
(assert(medium-energy-production_no-more-than-10_check-the-visual-impact_close(read)))
)

(defrule while-medium-energy-and-no-more-than-10-and-quiet
(medium-energy-production_no-more-than-10_check-the-visual-impact_close 1)
(not(medium-energy-production_no-more-than-10_check-the-visual-impact_close_quiet ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable or partly stable or 2.Unstable)?")
(assert(medium-energy-production_no-more-than-10_check-the-visual-impact_close_quiet(read)))
)

(defrule while-medium-energy-and-no-more-than-10-and-quiet-check-the-geological-survey
(medium-energy-production_no-more-than-10_check-the-visual-impact_close_quiet 1)
=>
(printout t "Accept as second best quality location - stabilise the land and/or create an alternative wildlife habitat if necessary")
)

(defrule while-medium-energy-and-no-more-than-10-check-the-geological-survey
(medium-energy-production_no-more-than-10_check-the-visual-impact_close_quiet 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-medium-energy-and-no-more-than-10-loud
(medium-energy-production_no-more-than-10_check-the-visual-impact_close 2)
=>
(printout t "Reject - completely unsuitable due to impact on population")
)

(defrule while-medium-energy-and-no-more-than-10-fairly-distant
(medium-energy-production_no-more-than-10_check-the-visual-impact 2)
(not(medium-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant ?))
=>
(printout t "Is the sound impact: (1.Stable or partly stable or 2.Unstable)?")
(assert(medium-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant(read)))
)

(defrule while-medium-energy-and-fairly-distant-and-unstable
(medium-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant 2)
=>
(printout t "Reject - completely unsuitable due to unstable ground")
)

(defrule while-medium-energy-and-fairly-distant-is-it-stable-or-partly-stable 
(medium-energy-production_no-more-than-10_check-the-visual-impact_fairly-distant 1)
=>
(printout t "Accept as second best quality location - stabilize the land if necessary")
)

(defrule while-medium-energy-and-very-distant
(medium-energy-production_no-more-than-10_check-the-visual-impact 3)
(not(medium-energy-production_no-more-than-10_check-the-visual-impact_very-distant ?))
=>
(printout t "Does the geological survey suggest that the ground is: (1.Stable 2.partly stable or 3.Unstable)?")
(assert(medium-energy-production_no-more-than-10_check-the-visual-impact_very-distant(read)))
)

(defrule while-medium-energy-and-no-more-than-10-unstable
(medium-energy-production_no-more-than-10_check-the-visual-impact_very-distant 3)
=>
(printout t "Reject - completely unsuitable due to unstable due to unstable ground")
)

(defrule while-medium-energy-and-no-more-than-10-partly-stable
(medium-energy-production_no-more-than-10_check-the-visual-impact_very-distant 2)
=>
(printout t "Accept as second best quality location - stabilize the land if necessary")
)

(defrule while-medium-energy-and-no-more-than-10-stable
(medium-energy-production_no-more-than-10_check-the-visual-impact_very-distant 1)
=>
(printout t "Accept as second best quality location - stabilize the land if necessary")
)























